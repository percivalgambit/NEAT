#include "shared/normal_floating_point_implementation.h"

#include <pin.H>

#include <iostream>

#include "shared/floating_point_implementation.h"
#include "shared/program_state.h"

namespace ftrace {

FLT32 NormalFloatingPointImplementation::FloatingPointOperation(
    const FLT32 &operand1, const FLT32 &operand2, const OPCODE &operation,
    const ProgramState &program_state) {
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

}  // namespace ftrace
