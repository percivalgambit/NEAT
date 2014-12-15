
/*! @file
 *  This is a PIN tool that traces every floating-point arithmetic operation and
 * displays the arguments and results.
 * @note All float values are printed as 8 digit hex numbers padded with 0's.
 */

#include "pin.H"
#include <fstream>
#include <iomanip>
#include <iostream>

/* ================================================================== */
// Global variables
/* ================================================================== */

/* ===================================================================== */
// Command line switches
/* ===================================================================== */

/**!
 *  Turn instrumentation on or off for the program.
 */
KNOB<BOOL> KnobInstrument(KNOB_MODE_WRITEONCE, "pintool", "instrument", "1",
                          "turn floating-point instruction tracing on or off");

/**!
 *  Turn floating-point instruction replacing on or off for the program.
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
 * Print the name of a floating-point instruction and its operands.
 * This function is called for every floating-point arithmetic instruction that
 * operates on two registers, when it is about to be executed.
 * @param[in]   op          the opcode of the floating-point instruction
 * @param[in]   operand1    the first operand of the instruction
 * @param[in]   operand2    the second operand of the instruction
 * @param[in]   ctxt        the context of the instrumented application immediately
 *                          before the instruction is executed
 */
VOID print_reg_fargs(OPCODE op, REG operand1, REG operand2, CONTEXT *ctxt) {
    PIN_REGISTER reg1, reg2;

    PIN_GetContextRegval(ctxt, operand1, reg1.byte);
    PIN_GetContextRegval(ctxt, operand2, reg2.byte);

    cout << OPCODE_StringShort(op) << " ";
    cout << setw(8) << setfill('0') << *(UINT32 *)reg1.flt << " ";
    cout << setw(8) << setfill('0') << *(UINT32 *)reg2.flt << endl;

#ifdef REPLACE_FP_FN
    if (KnobReplaceFPIns) {
        PIN_REGISTER result;

        *result.flt = REPLACE_FP_FN(*(FLT32 *)reg1.flt, *(FLT32 *)reg2.flt, op);
        PIN_SetContextRegval(ctxt, operand1, result.byte);
    }
#endif
}

/*!
 * Print the name of a floating-point instruction and its operands.
 * This function is called for every floating-point arithmetic instruction that
 * operates on a register and a memory location, when it is about to be executed.
 * @param[in]   op          the opcode of the floating-point instruction
 * @param[in]   operand1    the first operand of the instruction
 * @param[in]   operand2    the second operand of the instruction
 * @param[in]   ctxt        the context of the instrumented application immediately
 *                          before the instruction is executed
 */
VOID print_mem_fargs(OPCODE op, REG operand1, ADDRINT *operand2, CONTEXT *ctxt) {
    PIN_REGISTER reg1;

    PIN_GetContextRegval(ctxt, operand1, reg1.byte);

    cout << OPCODE_StringShort(op) << " ";
    cout << setw(8) << setfill('0') << *(UINT32 *)reg1.flt << " ";
    cout << setw(8) << setfill('0') << *(UINT32 *)operand2 << endl;

#ifdef REPLACE_FP_FN
    if (KnobReplaceFPIns) {
        PIN_REGISTER result;

        *result.flt = REPLACE_FP_FN(*(FLT32 *)reg1.flt, *(FLT32 *)operand2, op);
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

    cout << "  ";
    cout << setw(8) << setfill('0') << *(UINT32 *)result.flt << endl;
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
            return true;
        default:
            return false;
    }
}

/*!
 * Insert calls to the analysis routines before and after every floating-point
 * instruction of the instrumented application.
 * This function is called every time a new instruction is encountered.
 * @param[in]   ins     instruction to be instrumented
 * @param[in]   v       value specified by the tool in the INS_AddInstrumentFunction
 *                      function call
 */
VOID Trace(INS ins, VOID *v) {
    if (isFpInstruction(ins)) {

#ifdef REPLACE_FP_FN
        if (KnobReplaceFPIns)
            INS_Delete(ins);
#endif

        REGSET regsIn, regsOut;
        REGSET_Clear(regsIn);
        REGSET_Clear(regsOut);
        REGSET_Insert(regsIn, REG(INS_OperandReg(ins, 0)));
        REGSET_Insert(regsOut, REG(INS_OperandReg(ins, 0)));

        if (INS_OperandIsReg(ins, 1)) {
            REGSET_Insert(regsIn, REG(INS_OperandReg(ins, 1)));

            // If an instruction is a floating-point instruction that operates on two
            // registers, call print_reg_fargs and pass it the two operands
            INS_InsertCall(ins,
                           IPOINT_BEFORE,
                           AFUNPTR(print_reg_fargs),
                           IARG_UINT32,
                           INS_Opcode(ins),
                           IARG_UINT32,
                           REG(INS_OperandReg(ins, 0)),
                           IARG_UINT32,
                           REG(INS_OperandReg(ins, 1)),
                           IARG_PARTIAL_CONTEXT,
                           &regsIn,
                           &regsOut,
                           IARG_END);
        }
        else {
            // If an instruction is a floating-point instruction that operates on a
            // register and a memory location, call print_mem_fargs and pass it the
            // two operands
            INS_InsertCall(ins,
                           IPOINT_BEFORE,
                           AFUNPTR(print_mem_fargs),
                           IARG_UINT32,
                           INS_Opcode(ins),
                           IARG_UINT32,
                           REG(INS_OperandReg(ins, 0)),
                           IARG_MEMORYREAD_EA,
                           IARG_PARTIAL_CONTEXT,
                           &regsIn,
                           &regsOut,
                           IARG_END);
        }


        // Call print_fresult after every floating-point instruction, and pass it the
        // result of the instruction
        INS_InsertCall(ins,
                       IPOINT_AFTER,
                       AFUNPTR(print_fresult),
                       IARG_UINT32,
                       REG(INS_OperandReg(ins, 0)),
                       IARG_CONTEXT,
                       IARG_END);
    }
}

/*!
 * This function is called when the application exits.
 * @param[in]   code            exit code of the application
 * @param[in]   v               value specified by the tool in the
 *                              PIN_AddFiniFunction function call
 */
VOID Fini(INT32 code, VOID *v) {}

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
        // Register Trace to be called to instrument instructions
        INS_AddInstrumentFunction(Trace, 0);

        // Register Fini to be called when the application exits
        PIN_AddFiniFunction(Fini, 0);

        // Make sure all values are printed as hex numbers
        cout << hex;
    }

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
