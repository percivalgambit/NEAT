#include "client_lib/interfaces/fp_implementation.h"

#include <pin.H>

#include <iostream>

namespace ftrace {

FLT32 FpImplementation::FpOperation(const FLT32 &operand1,
                                    const FLT32 &operand2,
                                    const OPCODE &operation) {
  switch (operation) {
    case XED_ICLASS_ADDSS:
      return FpAdd(operand1, operand2);
    case XED_ICLASS_SUBSS:
      return FpSub(operand1, operand2);
    case XED_ICLASS_MULSS:
      return FpMul(operand1, operand2);
    case XED_ICLASS_DIVSS:
      return FpDiv(operand1, operand2);
    default:
      std::cerr << "Unexpected opcode " << operation
                << " encountered when replacing floating-point instructions"
                << endl;
      return operand1;
  }
}

}  // namespace ftrace
