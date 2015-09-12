#ifndef PINTOOL_FTRACE_H_
#define PINTOOL_FTRACE_H_

#include <pin.H>

#include <cstdlib>
#include <iostream>
#include <string>

#include "client/floating_point_implementation.h"

#define ERROR(message)                                                        \
    do {                                                                      \
      cerr << message << endl;                                                \
      exit(1);                                                                \
    } while (0)                                                               \

/*!
 * Convert a FLT32 variable into the string representation of its value in hex.
 * @param[in]   fp  variable to convert to hex
 */
#define FLT32_TO_HEX(fp)                                                      \
    StringHex(*reinterpret_cast<const UINT32 *>(&fp), 8, FALSE)               \

namespace ftrace {

struct RoutineInstrumentationArgs {
  RoutineInstrumentationArgs(
      FloatingPointImplementation *floating_point_impl,
      const BOOL print_floating_point_ops) :
          floating_point_impl_(floating_point_impl),
          print_floating_point_ops_(print_floating_point_ops) {}

  FloatingPointImplementation *floating_point_impl_;
  const BOOL print_floating_point_ops_;
};

/**
 * Insert calls to the analysis routines before and after every floating-point
 * instruction of the instrumented application.  If function-level replacement
 * types are enabled, then every routine will also be instrumented with functions
 * to push and pop its replacement type from the stack hen it is entered and
 * exited.
 * This function is called every time a new routine is encountered.
 * @param[in]   rtn      routine to be instrumented
 * @param[in]   v        value specified by the tool in the
 *                       RTN_AddInstrumentFunction function call
 */
VOID Routine(const RTN rtn, VOID *routine_instrumentation_args);

}  // namespace ftrace

#endif  // PINTOOL_FTRACE_H_
