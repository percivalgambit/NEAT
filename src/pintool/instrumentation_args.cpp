#include "pintool/instrumentation_args.h"

#include <pin.H>

#include <fstream>

#include "shared/floating_point_implementation_selector.h"

namespace ftrace {

InstrumentationArgs::InstrumentationArgs(
    const BOOL &print_floating_point_ops, ofstream *output_stream,
    FloatingPointImplementationSelector *floating_point_implementation_selector)
    : print_floating_point_ops(print_floating_point_ops),
      output_stream(output_stream),
      floating_point_implementation_selector(floating_point_implementation_selector) {}

InstrumentationArgs::~InstrumentationArgs() {
  output_stream->close();
  delete output_stream;
}

}  // namespace ftrace
