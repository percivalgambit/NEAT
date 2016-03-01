#ifndef TESTS_COMPLEX_TEST_FLOATING_POINT_IMPLEMENTATION_H_
#define TESTS_COMPLEX_TEST_FLOATING_POINT_IMPLEMENTATION_H_

/*! @file
 * This is a more complex replacement function for arithmetic floating-point
 * instructions in the ftrace tool.
 */

#include <pin.H>

#include "shared/floating_point_implementation.h"

#include <iostream>

namespace ftrace {

class ComplexTestFloatingPointImplementation
    : public FloatingPointImplementation {
 public:
  /*!
   * A complex implementation of floating-point arithmetic operations.
   */
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation) override {
    switch (operation) {
      case XED_ICLASS_ADDSS:
        return (operand1 + operand2) * 0.9;
      case XED_ICLASS_SUBSS:
        return (operand1 - operand2) * 0.9;
      case XED_ICLASS_MULSS:
        return (operand1 * operand2) * 0.9;
      case XED_ICLASS_DIVSS:
        return (operand1 / operand2) * 0.9;
      default:
        std::cerr << "Unexpected opcode " << operation
                  << " encountered "
                     "when replacing floating-point instructions"
                  << endl;
        return operand1;
    }
  }
};

}  // namespace ftrace

#endif  // TESTS_COMPLEX_TEST_FLOATING_POINT_IMPLEMENTATION_H_
