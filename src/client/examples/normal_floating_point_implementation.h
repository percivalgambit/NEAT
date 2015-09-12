#ifndef CLIENT_EXAMPLES_NORMAL_FLOATING_POINT_IMPLEMENTATION_H_
#define CLIENT_EXAMPLES_NORMAL_FLOATING_POINT_IMPLEMENTATION_H_

#include <pin.H>

#include <iostream>

#include "client/floating_point_implementation.h"

namespace ftrace {

class NormalFloatingPointImplementation : public FloatingPointImplementation {
 public:
  FLT32 FloatingPointOperation(
      const FLT32 operand1, const FLT32 operand2, const OPCODE operation,
      const UINT32 replace_type) override {
    switch (operation) {
      case XED_ICLASS_ADDSS:
        return operand1 + operand2;
      case XED_ICLASS_SUBSS:
        return operand1 - operand2;
      case XED_ICLASS_MULSS:
        return operand1 * operand2;
      case XED_ICLASS_DIVSS:
        return operand1 / operand2;
      default:
        cerr << "Unexpected opcode " << operation << "encountered when "
             << "replacing floating-point instructions" << endl;
        return operand1;
    }
  }
};

}  // namespace ftrace

#endif  // CLIENT_EXAMPLES_NORMAL_FLOATING_POINT_IMPLEMENTATION_H_
