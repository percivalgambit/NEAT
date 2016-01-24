#ifndef PINTOOL_INSTRUMENTATION_ARGS_H_
#define PINTOOL_INSTRUMENTATION_ARGS_H_

#include <pin.H>

#include <fstream>

#include "shared/floating_point_implementation_selector.h"

namespace ftrace {

/**
 * Stores invocation-specific information that is passed to the instrumentation
 * functions that are used to instrument the application program.
 */
struct InstrumentationArgs {
  InstrumentationArgs(
      const BOOL &print_floating_point_ops, ofstream *output_stream,
      FloatingPointImplementationSelector *floating_point_implementation_selector);

  ~InstrumentationArgs();

  /// True if the pintool should print the operands and result of each
  /// floating-point operation in the instrumented application.
  const BOOL print_floating_point_ops;
  /// File where all output of the pintool will be written.
  ofstream *output_stream;
  /// Implementation of floating-point arithmetic with which to replace every
  /// floating-point arithmetic instruction in the instrumented program.
  FloatingPointImplementationSelector *floating_point_implementation_selector;
};

}  /// namespace ftrace

#endif  // PINTOOL_INSTRUMENTATION_ARGS_H_
