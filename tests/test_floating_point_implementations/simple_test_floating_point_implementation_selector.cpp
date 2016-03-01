#include "simple_test_floating_point_implementation.h"

#include "client/single_floating_point_implementation_selector.h"
#include "shared/floating_point_implementation_selector.h"

namespace ftrace {

REGISTER_FLOATING_POINT_IMPL_SELECTOR(SingleFloatingPointImplementationSelector<SimpleTestFloatingPointImplementation>);

}  // namespace ftrace
