
/*! @file
 *  This is an example of the PIN tool that demonstrates some basic PIN APIs
 *  and could serve as the starting point for developing your first PIN tool
 */

#include <stdio.h>
#include "pin.H"

/* ================================================================== */
// Global variables
/* ================================================================== */

FILE *trace;

/* ===================================================================== */
// Command line switches
/* ===================================================================== */

/* ===================================================================== */
// Utilities
/* ===================================================================== */

/*!
 *  Print out help message.
 */
INT32 Usage()
{
    PIN_ERROR("This tool produces a trace of floating point "
              "arithmetic and comparison instruction calls.\n"
              + KNOB_BASE::StringKnobSummary() + "\n");

    return -1;
}

/* ===================================================================== */
// Analysis routines
/* ===================================================================== */

/*!
 * Increase counter of the executed basic blocks and instructions.
 * This function is called for every basic block when it is about to be executed.
 * @param[in]   numInstInBbl    number of instructions in the basic block
 * @note use atomic operations for multi-threaded applications
 */
VOID print_fargs(CHAR *op, PIN_REGISTER *arg1, PIN_REGISTER *arg2)
{
    fprintf(trace, "%s %f %f\n", op, *arg1->dbl, *arg2->dbl);
}

VOID print_fresult(PIN_REGISTER *result)
{
    fprintf(trace, "%f\n", *result->dbl);
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
    // Insert a call to print_fargs before every fp instruction, and pass it
    // the values of the top two fp stack registers

    // Insert a call to print_fresult after every fp instruction, and pass it
    // the values of the top fp stack register
}

// returns true if an instruction is an arithmentic or comparison floating-point instruction
BOOL isFpInstruction(INS ins)
{
    OPCODE op = INS_Opcode(ins);

    switch(op) {
        case XED_ICLASS_FADD:
        case XED_ICLASS_FSUB:
        case XED_ICLASS_FMUL:
        case XED_ICLASS_FDIV:
        case XED_ICLASS_FCOM:
            return true;
        default:
            return false;
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
    fprintf(trace, "#eof\n");
    fclose(trace);
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
