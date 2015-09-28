#ifndef PINTOOL_INSTRUMENTATION_ARGS_H_
#define PINTOOL_INSTRUMENTATION_ARGS_H_

#include <pin.H>

#include <fstream>

#include "client/interfaces/floating_point_implementation_generator.h"

namespace ftrace {

struct InstrumentationArgs {
  InstrumentationArgs(const BOOL &print_floating_point_ops,
                      ofstream *output_stream,
                      FloatingPointImplementationGenerator *fpig);

  ~InstrumentationArgs();

  const BOOL print_floating_point_ops_;
  ofstream *output_stream_;
  FloatingPointImplementationGenerator *fpig_;
};

}  /// namespace ftrace

#endif  // PINTOOL_INSTRUMENTATION_ARGS_H_
