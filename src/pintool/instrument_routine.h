#ifndef PINTOOL_INSTRUMENT_ROUTINE_H_
#define PINTOOL_INSTRUMENT_ROUTINE_H_

#include <pin.H>

#include "pintool/instrumentation_args.h"

namespace ftrace {

/**
 * Insert calls to the analysis routines before and after every floating-point
 * instruction of the instrumented application.  If function-level replacement
 * types are enabled, then every routine will also be instrumented with
 * functions
 * to push and pop its replacement type from the stack hen it is entered and
 * exited.
 * This function is called every time a new routine is encountered.
 * @param[in]   rtn      routine to be instrumented
 * @param[in]   v        value specified by the tool in the
 *                       RTN_AddInstrumentFunction function call
 */
VOID InstrumentRoutine(const RTN rtn,
                       const InstrumentationArgs *instrumentation_args);

}  // namespace ftrace

#endif  // PINTOOL_INSTRUMENT_ROUTINE_H_
