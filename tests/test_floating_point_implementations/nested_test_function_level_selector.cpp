#include <map>
#include <string>

#include "complex_test_floating_point_implementation.h"
#include "simple_test_floating_point_implementation.h"

#include "client/function_level_floating_point_implementation_selector.h"
#include "client/normal_floating_point_implementation.h"
#include "shared/floating_point_implementation_selector.h"
#include "shared/floating_point_implementation.h"

namespace ftrace {

SimpleTestFloatingPointImplementation simple;
ComplexTestFloatingPointImplementation complex;
NormalFloatingPointImplementation normal;

const map<string, FloatingPointImplementation *> function_name_map = {
    {"helper1", &simple},
    {"helper2", &simple},
    {"nested_helper", &complex},
};

REGISTER_FLOATING_POINT_IMPL_SELECTOR(FunctionLevelFloatingPointImplementationSelector, function_name_map, &normal);

}  // namespace ftrace
