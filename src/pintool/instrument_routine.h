#ifndef PINTOOL_INSTRUMENT_ROUTINE_H_
#define PINTOOL_INSTRUMENT_ROUTINE_H_

#include <pin.H>

#include "client_lib/interfaces/fp_selector.h"

namespace ftrace {

/**
 * Insert calls to analysis routines to replace every floating-point operation
 * in the instrumented application with a user-defined implementation
 * This function is called every time a new routine is encountered.
 * @param[in]   rtn                    routine to be instrumented
 * @param[in]   instrumentation_args   arguments passed to the instrumentation
 *                                     function
 */
VOID InstrumentFpOperations(const RTN rtn, FpSelector *fp_selector);

VOID PrintFpOperations(const INS rtn, ofstream *output_stream);

}  // namespace ftrace

#endif  // PINTOOL_INSTRUMENT_ROUTINE_H_
