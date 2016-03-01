#ifndef TESTS_SIMPLE_TEST_FLOATING_POINT_IMPLEMENTATION_H_
#define TESTS_SIMPLE_TEST_FLOATING_POINT_IMPLEMENTATION_H_

/*! @file
 * This is a simple replacement function for arithmetic floating-point
 * instructions
 * in the ftrace tool.
 */

#include <pin.H>

#include "shared/floating_point_implementation.h"

namespace ftrace {

class SimpleTestFloatingPointImplementation
    : public FloatingPointImplementation {
 public:
  /*!
   * A simple implementation of floating-point arithmetic operations.
   */
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation) override {
    return 1.0;
  }
};

}  // namespace ftrace

#endif  // TESTS_SIMPLE_TEST_FLOATING_POINT_IMPLEMENTATION_H_
