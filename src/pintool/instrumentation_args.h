#ifndef PINTOOL_INSTRUMENTATION_ARGS_H_
#define PINTOOL_INSTRUMENTATION_ARGS_H_

#include <pin.H>

#include <fstream>

#include "shared/floating_point_implementation.h"

namespace ftrace {

struct InstrumentationArgs {
  InstrumentationArgs(
      const BOOL &print_floating_point_ops, ofstream *output_stream,
      FloatingPointImplementation *floating_point_implementation);

  ~InstrumentationArgs();

  const BOOL print_floating_point_ops;
  ofstream *output_stream;
  FloatingPointImplementation *floating_point_implementation;
};

}  /// namespace ftrace

#endif  // PINTOOL_INSTRUMENTATION_ARGS_H_
