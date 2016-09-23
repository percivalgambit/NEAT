#include "client/normal_floating_point_implementation.h"

#include "client/register_fp_selector.h"
#include "client/register_single_fp_implementation_selector.h"

namespace ftrace {

static RegisterSingleFpImplementationSelector<NormalFloatingPointImplementation>
    fp_selector("default");

}  // namespace ftrace
