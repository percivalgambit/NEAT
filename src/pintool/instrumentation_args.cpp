#include "pintool/instrumentation_args.h"

#include <pin.H>

#include <fstream>

#include "client/floating_point_implementation.h"

namespace ftrace {

InstrumentationArgs::InstrumentationArgs(
    const BOOL &print_floating_point_ops, ofstream *output_stream,
    FloatingPointImplementation *floating_point_impl)
    : print_floating_point_ops_(print_floating_point_ops),
      output_stream_(output_stream),
      floating_point_impl_(floating_point_impl) {}

InstrumentationArgs::~InstrumentationArgs() {
  output_stream_->close();
  delete output_stream_;
}

}  // namespace ftrace
