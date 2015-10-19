
/*! @file
 * This is a simple replacement function for arithmetic floating-point instructions
 * in the ftrace tool.
 */

#include <pin.H>

#include "client/register_floating_point_implementation_generator.h"
#include "client/interfaces/floating_point_implementation.h"

namespace ftrace {

class SimpleTestFloatingPointImplementation : public FloatingPointImplementation {
 public:
  /*!
   * A simple implementation of floating-point arithmetic operations.
   */
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation) override {
    return 1.0;
  }
};

REGISTER_SINGLE_FLOATING_POINT_IMPL(SimpleTestFloatingPointImplementation);

}  // namespace ftrace
