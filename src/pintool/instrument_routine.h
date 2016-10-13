#ifndef PINTOOL_INSTRUMENT_ROUTINE_H_
#define PINTOOL_INSTRUMENT_ROUTINE_H_

#include <pin.H>

#include "client_lib/interfaces/fp_selector.h"

namespace ftrace {

/**
 * Schedule calls to analysis routines to replace every floating-point operation
 * in the instrumented application with a user-defined implementation.
 * This function is called every time a new routine is encountered, before the
 * instrumented application is run if the KnobFpSelectorName flag is supplied on
 * the command line.
 *
 * @param[in] rtn Routine to be instrumented.
 * @param[in] fp_selector The floating-point selector to use.
 */
VOID ReplaceFpOperations(const RTN rtn, FpSelector *fp_selector);

/**
 * Schedule calls to analysis routines to print the operands and result of every
 * floating-point instruction in the instrumented application.
 * This function is called every time a new instruction is encountered, before
 * the instrumented application is run if the KnobPrintFpOps flag is supplied
 * on the command line.
 *
 * @param[in] ins Instruction to be instrumented.
 * @param[in] output The output file to write to.
 */
VOID PrintFpOperations(const INS ins, ofstream *output);

}  // namespace ftrace

#endif  // PINTOOL_INSTRUMENT_ROUTINE_H_
