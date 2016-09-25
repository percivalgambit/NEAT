#include "client_lib/fp_selectors/normal_floating_point_implementation.h"

#include "client_lib/registry/register_single_fp_implementation_selector.h"

namespace ftrace {

static RegisterSingleFpImplementationSelector<NormalFloatingPointImplementation>
    fp_selector("default");

}  // namespace ftrace
