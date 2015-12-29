#include <pin.H>

#include <map>
#include <string>

#include "test_floating_point_implementation_functions.h"

#include "shared/floating_point_implementation.h"
#include "shared/normal_floating_point_implementation.h"
#include "shared/program_state.h"

namespace ftrace {

typedef FLT32 (*StatelessFloatingPointOperation)(
    const FLT32 &, const FLT32 &, const OPCODE &);

static NormalFloatingPointImplementation normal_floating_point_implementation;

static const map<string, StatelessFloatingPointOperation>
    function_operation_map = {
        {"helper1", SimpleTestFloatingPointOperation},
        {"helper2", SimpleTestFloatingPointOperation},
        {"nested_helper", ComplexTestFloatingPointOperation},
    };

class FunctionLevelNestedTestFloatingPointImplementation : public FloatingPointImplementation {
 public:
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation,
                               const ProgramState &program_state) override {
    for (auto function_name = program_state.function_stack_.crbegin();
         function_name != program_state.function_stack_.crend();
         ++function_name) {
      if (function_operation_map.find(*function_name) != function_operation_map.end()) {
        return function_operation_map.at(*function_name)(operand1, operand2, operation);
      }
    }

    return normal_floating_point_implementation.FloatingPointOperation(
        operand1, operand2, operation, program_state);
  }
};

REGISTER_FLOATING_POINT_IMPL(FunctionLevelNestedTestFloatingPointImplementation);

}  // namespace ftrace
