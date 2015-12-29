#include "test_floating_point_implementation_functions.h"

#include <pin.H>

#include <iostream>

#include "shared/program_state.h"

namespace ftrace {

FLT32 SimpleTestFloatingPointOperation(
    const FLT32 &operand1, const FLT32 &operand2, const OPCODE &operation) {
  return 1.0;
}

FLT32 ComplexTestFloatingPointOperation(
    const FLT32 &operand1, const FLT32 &operand2, const OPCODE &operation) {
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
      std::cerr << "Unexpected opcode " << operation << " encountered "
                    "when replacing floating-point instructions" << endl;
      return operand1;
  }
}

}  // namespace ftrace
