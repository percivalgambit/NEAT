#ifndef TEST_FLOATING_POINT_IMLEMENTATION_FUNCTIONS_H_
#define TEST_FLOATING_POINT_IMLEMENTATION_FUNCTIONS_H_

#include <pin.H>

#include "shared/program_state.h"

namespace ftrace {

class SimpleTestFloatingPointImplementation
    : public FloatingPointImplementation {
 public:
  /*!
   * A simple implementation of floating-point arithmetic operations.
   */
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation) override;
};

class ComplexTestFloatingPointImplementation
    : public FloatingPointImplementation {
 public:
  /*!
   * A complex implementation of floating-point arithmetic operations.
   */
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation) override;
  }
};

}  // namespace ftrace

#endif  // TEST_FLOATING_POINT_IMLEMENTATION_FUNCTIONS_H_
