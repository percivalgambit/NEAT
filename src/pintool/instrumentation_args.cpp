#include "pintool/instrumentation_args.h"

#include <pin.H>

#include <fstream>

#include "client/interfaces/floating_point_implementation_generator.h"

namespace ftrace {

InstrumentationArgs::InstrumentationArgs(
    const BOOL &print_floating_point_ops, ofstream *output_stream,
    FloatingPointImplementationGenerator *fpig)
    : print_floating_point_ops_(print_floating_point_ops),
      output_stream_(output_stream),
      fpig_(fpig) {}

InstrumentationArgs::~InstrumentationArgs() {
  output_stream_->close();
  delete output_stream_;
}

}  // namespace ftrace
