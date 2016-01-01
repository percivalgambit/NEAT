#include "pintool/instrumentation_args.h"

#include <pin.H>

#include <fstream>

#include "shared/floating_point_implementation.h"

namespace ftrace {

InstrumentationArgs::InstrumentationArgs(
    const BOOL &print_floating_point_ops, ofstream *output_stream,
    FloatingPointImplementation *floating_point_implementation)
    : print_floating_point_ops(print_floating_point_ops),
      output_stream(output_stream),
      floating_point_implementation(floating_point_implementation) {}

InstrumentationArgs::~InstrumentationArgs() {
  output_stream->close();
  delete output_stream;
}

}  // namespace ftrace
