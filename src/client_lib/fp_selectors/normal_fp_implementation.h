#ifndef CLIENT_LIB_FP_SELECTORS_NORMAL_FP_IMPLEMENTATION_H_
#define CLIENT_LIB_FP_SELECTORS_NORMAL_FP_IMPLEMENTATION_H_

#include <pin.H>

#include <iostream>

#include "client_lib/interfaces/fp_implementation.h"
#include "client_lib/registry/register_fp_selector.h"

namespace ftrace {

/**
 * A default implementation of floating-point arithmetic that performs each
 * operation normally.
 */
class NormalFpImplementation : public FpImplementation {
 public:
  FLT32 FpOperation(const FLT32 &operand1, const FLT32 &operand2,
                    const OPCODE &operation) {
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

#endif  // CLIENT_LIB_FP_SELECTORS_NORMAL_FP_IMPLEMENTATION_H_
