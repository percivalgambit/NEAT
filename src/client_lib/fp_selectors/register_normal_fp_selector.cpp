#include "client_lib/fp_selectors/normal_fp_implementation.h"

#include "client_lib/registry/register_single_fp_implementation_selector.h"

namespace ftrace {

static RegisterSingleFpImplementationSelector<NormalFpImplementation>
    fp_selector("default");

}  // namespace ftrace
