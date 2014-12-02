
/*! @file
 *  This is an example of the PIN tool that demonstrates some basic PIN APIs
 *  and could serve as the starting point for developing your first PIN tool
 */

#include "pin.H"
#include <iostream>
#include <fstream>

/* ================================================================== */
// Global variables
/* ================================================================== */

std::ofstream TraceFile;

/* ===================================================================== */
// Command line switches
/* ===================================================================== */

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
    "o", "ftrace.out", "specify trace file name");

/* ===================================================================== */
// Utilities
/* ===================================================================== */

/*!
 *  Print out help message.
 */
INT32 Usage()
{
    cerr << "This tool produces a trace of floating point "
            "arithmetic and comparison instruction calls." << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
// Analysis routines
/* ===================================================================== */

// returns true if an instruction is an arithmentic or comparison floating-point instruction
BOOL isFpInstruction(INS ins)
{
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
 * Increase counter of the executed basic blocks and instructions.
 * This function is called for every basic block when it is about to be executed.
 * @param[in]   numInstInBbl    number of instructions in the basic block
 * @note use atomic operations for multi-threaded applications
 */
VOID print_reg_fargs(OPCODE op, REG operand1, REG operand2, CONTEXT *ctxt)
{
    PIN_REGISTER reg1, reg2;

    PIN_GetContextRegval(ctxt, operand1, (UINT8 *)reg1.byte);
    PIN_GetContextRegval(ctxt, operand2, (UINT8 *)reg2.byte);

    TraceFile << OPCODE_StringShort(op) << " " << *reg1.flt << " " << *reg2.flt << endl;
}

VOID print_mem_fargs(OPCODE op, REG operand1, ADDRINT *operand2, CONTEXT *ctxt)
{
    PIN_REGISTER reg1;

    PIN_GetContextRegval(ctxt, operand1, (UINT8 *)reg1.byte);

    TraceFile << OPCODE_StringShort(op) << " " << *reg1.flt << " " << *(float *)operand2 << endl;
}

VOID print_fresult(REG operand2, CONTEXT *ctxt)
{
    PIN_REGISTER result;

    PIN_GetContextRegval(ctxt, operand2, (UINT8 *)result.byte);

    TraceFile << "  " << *result.flt << endl;
}

/* ===================================================================== */
// Instrumentation callbacks
/* ===================================================================== */

/*!
 * Insert call to the CountBbl() analysis routine before every basic block
 * of the trace.
 * This function is called every time a new trace is encountered.
 * @param[in]   trace    trace to be instrumented
 * @param[in]   v        value specified by the tool in the TRACE_AddInstrumentFunction
 *                       function call
 */
VOID Trace(INS ins, VOID *v)
{
    if (isFpInstruction(ins))
    {
        // Insert a call to print_fargs before every fp instruction, and pass it
        // the values of the top two fp stack registers
        if (INS_OperandIsReg(ins, 1))
        {
            INS_InsertCall(ins,
                           IPOINT_BEFORE,
                           AFUNPTR(print_reg_fargs),
                           IARG_UINT32,
                           INS_Opcode(ins),
                           IARG_UINT32,
                           REG(INS_OperandReg(ins, 0)),
                           IARG_UINT32,
                           REG(INS_OperandReg(ins, 1)),
                           IARG_CONTEXT,
                           IARG_END);
        }
        else
        {
            INS_InsertCall(ins,
                           IPOINT_BEFORE,
                           AFUNPTR(print_mem_fargs),
                           IARG_UINT32,
                           INS_Opcode(ins),
                           IARG_UINT32,
                           REG(INS_OperandReg(ins, 0)),
                           IARG_MEMORYREAD_EA,
                           IARG_CONTEXT,
                           IARG_END);
        }

        // Insert a call to print_fresult after every fp instruction, and pass it
        // the values of the top fp stack register
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
 * Print out analysis results.
 * This function is called when the application exits.
 * @param[in]   code            exit code of the application
 * @param[in]   v               value specified by the tool in the
 *                              PIN_AddFiniFunction function call
 */
VOID Fini(INT32 code, VOID *v)
{
    TraceFile.close();
}

/*!
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet started.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments,
 *                              including pin -t <toolname> -- ...
 */
int main(int argc, char *argv[])
{
    // Initialize PIN library. Print help message if -h(elp) is specified
    // in the command line or the command line is invalid
    if( PIN_Init(argc,argv) )
    {
        return Usage();
    }

    // Write to a file since cout and cerr maybe closed by the application
    TraceFile.open(KnobOutputFile.Value().c_str());
    TraceFile << hex;
    TraceFile.setf(ios::showbase);

    // Register Trace to be called to instrument instructions
    INS_AddInstrumentFunction(Trace, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
