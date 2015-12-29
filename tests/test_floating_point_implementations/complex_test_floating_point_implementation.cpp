
/*! @file
 * This is a more complex replacement function for arithmetic floating-point
 * instructions in the ftrace tool.
 */

#include <pin.H>

#include "test_floating_point_implementation_functions.h"

#include "shared/floating_point_implementation.h"
#include "shared/program_state.h"

namespace ftrace {

class ComplexTestFloatingPointImplementation
    : public FloatingPointImplementation {
 public:
  /*!
   * A complex implementation of floating-point arithmetic operations.
   */
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation,
                               const ProgramState &program_state) override {
    return ComplexTestFloatingPointOperation(operand1, operand2, operation);
  }
};

REGISTER_FLOATING_POINT_IMPL(ComplexTestFloatingPointImplementation);

}  // namespace ftrace
