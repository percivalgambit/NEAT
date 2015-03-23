
/*! @file
 *  This is a PIN tool that traces every floating-point arithmetic operation and
 * displays the arguments and results.  This tool can also replace floating-point
 * instructions with arbitrary code to test different floating-point implementations.
 * @note All floating-point values are printed as 8 digit hex numbers padded with 0's.
 */

#include <fstream>
#include <iostream>
#include <stack>

#include "pin.H"

/* ================================================================== */
// Global variables
/* ================================================================== */

#ifdef REPLACE_FP_FN
FLT32 REPLACE_FP_FN(FLT32, FLT32, OPCODE, UINT32);
#endif

ofstream OutFile; /*!<  Output file for the pintool */

stack<UINT32> replacement_type_stack;

static UINT64 ins_count = 0; /*!< count of the total number of instructions in the
                                  instrumented program */
static UINT64 fp_count = 0; /*!< count of the total number of floating point
                                 instructions in the instrumented program */
static UINT64 instrumented_fp_count = 0; /*!< count of the number of instrumented
                                              floating point instructions in the
                                              instrumented program */

/* ===================================================================== */
// Command line switches
/* ===================================================================== */

/*!
 *  Specify the file name for the ftrace output file.
 */
KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE,  "pintool",
    "o", "", "specify file name for ftrace output");

/*!
 *  Turn instrumentation on or off for the program.
 */
KNOB<BOOL> KnobInstrument(KNOB_MODE_WRITEONCE, "pintool", "instrument", "1",
                          "turn floating-point instruction tracing on or off");

/*!
 *  Turn floating-point instruction replacing on or off for the program.
 *  @note Has no effect unless the pintool is compiled with the instruction
 *  replacing code.  The best way to do this is to define REPLACE_FP_FN in the
 *  makefile.
 */
KNOB<BOOL> KnobReplaceFPIns(KNOB_MODE_WRITEONCE, "pintool", "fp-replace", "1",
                          "turn floating-point instruction replacing on or off");

/* ===================================================================== */
// Utilities
/* ===================================================================== */

/*!
 *  Print out help message.
 */
INT32 Usage() {
    cerr << "This tool produces a trace of floating point "
            "arithmetic and comparison instruction calls." << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
// Analysis routines
/* ===================================================================== */

/*!
 * Increment a counter.  This is an analysis routine meant to be run every time
 * a apecified event happens in the instrumented program
 * @param[in,out]   counter     the counter that is incremented
 */
VOID docount(UINT64 *counter) {
    (*counter)++;
}

#ifdef REPLACE_FP_FN
VOID push_replacement_type(UINT32 replace_type) {
    if (replace_type != _no_replacement) {
        replacement_type_stack.push(replace_type);
    }
}

VOID pop_replacement_type(UINT32 replace_type) {
    if (replace_type != _no_replacement) {
        replacement_type_stack.pop();
    }
}
#endif

/*!
 * Record the name of a floating-point instruction and its operands. Replace
 * floating-point instructions with a user-specified function, if specified during
 * compilation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on two registers, when it is about to be executed.
 * @param[in]   op          the opcode of the floating-point instruction
 * @param[in]   operand1    the first operand of the instruction
 * @param[in]   operand2    the second operand of the instruction
 * @param[in]   ctxt        the context of the instrumented application immediately
 *                          before the instruction is executed
 * @param[in]   output      whether to print output to the output file
 */
VOID print_reg_fargs(OPCODE op, REG operand1, REG operand2, UINT32 replace_type,
                     CONTEXT *ctxt, BOOL output) {
    PIN_REGISTER reg1, reg2;

    PIN_GetContextRegval(ctxt, operand1, reg1.byte);
    PIN_GetContextRegval(ctxt, operand2, reg2.byte);

    if (output) {
        OutFile << OPCODE_StringShort(op)
                << " " << StringHex(*reg1.dword, 8, FALSE)
                << " " << StringHex(*reg2.dword, 8, FALSE)
                << "\n";
    }

#ifdef REPLACE_FP_FN
    if (KnobReplaceFPIns) {
        PIN_REGISTER result;

        *result.flt = REPLACE_FP_FN(*reg1.flt, *reg2.flt, op, replace_type);
        PIN_SetContextRegval(ctxt, operand1, result.byte);
    }
#endif
}

/*!
 * Record the name of a floating-point instruction and its operands. Replace
 * floating-point instructions with a user-specified function, if specified during
 * compilation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on a register and a memory location, when it is about to be executed.
 * @param[in]   op          the opcode of the floating-point instruction
 * @param[in]   operand1    the first operand of the instruction
 * @param[in]   operand2    the second operand of the instruction
 * @param[in]   ctxt        the context of the instrumented application immediately
 *                          before the instruction is executed
 * @param[in]   output      whether to print output to the output file
 */
VOID print_mem_fargs(OPCODE op, REG operand1, ADDRINT *operand2, UINT32 replace_type,
                     CONTEXT *ctxt, BOOL output) {
    PIN_REGISTER reg1;

    PIN_GetContextRegval(ctxt, operand1, reg1.byte);

    if (output) {
        OutFile << OPCODE_StringShort(op)
                << " " << StringHex(*reg1.dword, 8, FALSE)
                << " " << StringHex(*operand2, 8, FALSE)
                << "\n";
    }

#ifdef REPLACE_FP_FN
    if (KnobReplaceFPIns) {
        PIN_REGISTER result;

        *result.flt = REPLACE_FP_FN(*reg1.flt, *(FLT32 *)operand2, op, replace_type);
        PIN_SetContextRegval(ctxt, operand1, result.byte);
    }
#endif
}

/*!
 * Print the result of a floating-point instruction.
 * This function is called for every floating-point arithmetic instruction after it
 * has finished executing.
 * @param[in]   operand1    the register where the result of the instruction is stored
 * @param[in]   ctxt        the context of the instrumented application immediately
 *                          after the instruction is executed
 */
VOID print_fresult(REG operand1, CONTEXT *ctxt) {
    PIN_REGISTER result;

    PIN_GetContextRegval(ctxt, operand1, result.byte);

    OutFile << "  " << StringHex(*result.dword, 8, FALSE) << "\n";
}

/* ===================================================================== */
// Instrumentation callbacks
/* ===================================================================== */

/*!
 * Return true if an instruction is an SSE floating-point arithmetic instruction.
 * This function is called on every instruction.
 * @param[in]   ins     the instruction to test
 * @return true if the instruction is an SSE floating-point arithmetic instruction,
 *         false if not
 * @note Currently, the instructions defined as SSE floating-point arithmetic
 *       operations are:
 *  - ADDSS
 *  - SUBSS
 *  - MULSS
 *  - DIVSS
 */
BOOL isFpInstruction(INS ins) {
    OPCODE op = INS_Opcode(ins);

    switch(op) {
        case XED_ICLASS_ADDSS:
        case XED_ICLASS_SUBSS:
        case XED_ICLASS_MULSS:
        case XED_ICLASS_DIVSS:
            return TRUE;
        default:
            return FALSE;
    }
}

/*!
 * Insert calls to the analysis routines before and after every floating-point
 * instruction of the instrumented application in selected routines.
 * This function is called every time a new instruction is encountered.
 * @param[in]   rtn     routine to be instrumented
 * @param[in]   output  pointer to a bool value that determines whether any
 *                      output should be produced by the instrumentation
 * @note To select which routines to instument, specify each routine as the
 *       argument to a -filter_rtn flag on the command line
 */
VOID Routine(RTN rtn, VOID *output) {
    RTN_Open(rtn);

#ifdef REPLACE_FP_FN
    RTN_InsertCall(rtn,
                   IPOINT_BEFORE,
                   (AFUNPTR)push_replacement_type,
                   IARG_UINT32,
                   get_replacement_type(RTN_Name(rtn)),
                   IARG_END);
#endif

    // Forward pass over all instructions in routine
    for(INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)) {

        if (*(BOOL *)output) {
            // Increment the count of the total number of instructions
            INS_InsertCall(ins,
                           IPOINT_BEFORE,
                           (AFUNPTR)docount,
                           IARG_PTR,
                           &ins_count,
                           IARG_END);
        }

        if (isFpInstruction(ins)) {

            if (*(BOOL *)output) {
                // Increment the count of floating point instructions
                INS_InsertCall(ins,
                               IPOINT_BEFORE,
                               (AFUNPTR)docount,
                               IARG_PTR,
                               &fp_count,
                               IARG_END);
            }

#ifdef REPLACE_FP_FN
            if (KnobReplaceFPIns)
                INS_Delete(ins);
#endif

            REGSET regsIn, regsOut;
            REGSET_Clear(regsIn);
            REGSET_Clear(regsOut);
            REGSET_Insert(regsIn, REG(INS_OperandReg(ins, 0)));
            REGSET_Insert(regsOut, REG(INS_OperandReg(ins, 0)));

            if (*(BOOL *)output) {
                // Increment the count of instrumented floating point instructions
                INS_InsertCall(ins,
                               IPOINT_BEFORE,
                               (AFUNPTR)docount,
                               IARG_PTR,
                               &instrumented_fp_count,
                               IARG_END);
            }

            if (INS_OperandIsReg(ins, 1)) {
                REGSET_Insert(regsIn, REG(INS_OperandReg(ins, 1)));

                // If an instruction is a floating-point instruction that operates on two
                // registers, call print_reg_fargs and pass it the two operands
                INS_InsertCall(ins,
                               IPOINT_BEFORE,
                               (AFUNPTR)print_reg_fargs,
                               IARG_UINT32,
                               INS_Opcode(ins),
                               IARG_UINT32,
                               INS_OperandReg(ins, 0),
                               IARG_UINT32,
                               INS_OperandReg(ins, 1),
                               IARG_UINT32,
#ifdef REPLACE_FP_FN
                               replacement_type_stack.empty() ? _no_replacement
                                                              : replacement_type_stack.top(),
#else
                               0,
#endif
                               IARG_PARTIAL_CONTEXT,
                               &regsIn,
                               &regsOut,
                               IARG_BOOL,
                               *(BOOL *)output,
                               IARG_END);
            }
            else {
                // If an instruction is a floating-point instruction that operates on a
                // register and a memory location, call print_mem_fargs and pass it the
                // two operands
                INS_InsertCall(ins,
                               IPOINT_BEFORE,
                               (AFUNPTR)print_mem_fargs,
                               IARG_UINT32,
                               INS_Opcode(ins),
                               IARG_UINT32,
                               INS_OperandReg(ins, 0),
                               IARG_MEMORYREAD_EA,
                               IARG_UINT32,
#ifdef REPLACE_FP_FN
                               replacement_type_stack.empty() ? _no_replacement
                                                              : replacement_type_stack.top(),
#else
                               0,
#endif
                               IARG_PARTIAL_CONTEXT,
                               &regsIn,
                               &regsOut,
                               IARG_BOOL,
                               *(BOOL *)output,
                               IARG_END);
            }

            if (*(BOOL *)output) {
                // Call print_fresult after every floating-point instruction, and pass it the
                // result of the instruction
                INS_InsertCall(ins,
                               IPOINT_AFTER,
                               (AFUNPTR)print_fresult,
                               IARG_UINT32,
                               INS_OperandReg(ins, 0),
                               IARG_CONST_CONTEXT,
                               IARG_END);
            }
        }
    }

#ifdef REPLACE_FP_FN
    RTN_InsertCall(rtn,
                   IPOINT_AFTER,
                   (AFUNPTR)pop_replacement_type,
                   IARG_UINT32,
                   get_replacement_type(RTN_Name(rtn)),
                   IARG_END);
#endif

    RTN_Close(rtn);
}

/*!
 * This function is called when the application exits.
 * @param[in]   code            exit code of the application
 * @param[in]   v               value specified by the tool in the
 *                              PIN_AddFiniFunction function call
 */
VOID Fini(INT32 code, VOID *v) {
    // Write to a file since cout and cerr maybe closed by the application
    OutFile.setf(ios::showbase);
    OutFile << "-----------------------" << endl;
    OutFile << "Total number of instructions: " << ins_count << endl;
    OutFile << "Number of floating point instructions: " << fp_count << endl;
    OutFile << "Number of instrumented floating point instructions: "
            << instrumented_fp_count << endl;
    OutFile.close();
}

/*!
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet started.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments,
 *                              including pin -t <toolname> -- ...
 */
int main(int argc, char *argv[]) {
    // Initialize PIN library. Print help message if -h(elp) is specified
    // in the command line or the command line is invalid
    if( PIN_Init(argc,argv) ) {
        return Usage();
    }

    if (KnobInstrument) {
        BOOL output;

        // If an output file was specified on the command line, instrument the
        // program for output, otherwise instrument the program for no output if
        // a floating point replacement function was specified during compilation
        if (!KnobOutputFile.Value().empty()) {
            output = TRUE;

            OutFile.open(KnobOutputFile.Value().c_str());

            cerr <<  "===============================================" << endl;
            cerr << "See file " << KnobOutputFile.Value() << " for analysis results" << endl;
            cerr <<  "===============================================" << endl;
        }
        else {
#ifdef REPLACE_FP_FN
            output = FALSE;
#endif
        }

        // Register Routine to be called to instrument instructions
        RTN_AddInstrumentFunction(Routine, &output);
    }
    else {
        PIN_Detach();
    }

#ifdef START_CALLBACK
    PIN_AddApplicationStartFunction(START_CALLBACK, 0);
#endif

#ifdef EXIT_CALLBACK
    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(EXIT_CALLBACK, 0);
#endif

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
