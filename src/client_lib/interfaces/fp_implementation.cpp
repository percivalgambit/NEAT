#include "client_lib/interfaces/fp_implementation.h"

#include <pin.H>

#include <cstdlib>
#include <iostream>

#include "client_lib/utils/fp_operation.h"

namespace ftrace {

FLT32 FpImplementation::PerformOperation(const FpOperation &operation) {
  switch (operation.opcode) {
    case XED_ICLASS_ADDSS:
      return FpAdd(operation);
    case XED_ICLASS_SUBSS:
      return FpSub(operation);
    case XED_ICLASS_MULSS:
      return FpMul(operation);
    case XED_ICLASS_DIVSS:
      return FpDiv(operation);
    default:
      std::cerr << "Unexpected opcode " << operation.opcode
                << " encountered when replacing floating-point instructions"
                << endl;
      exit(1);
  }
}

}  // namespace ftrace
