#include "pintool/utils.h"

#include <pin.H>

namespace ftrace {

BOOL IsFpInstruction(const INS &ins) {
  const OPCODE op = INS_Opcode(ins);
  switch (op) {
    case XED_ICLASS_ADDSS:
    case XED_ICLASS_SUBSS:
    case XED_ICLASS_MULSS:
    case XED_ICLASS_DIVSS:
      return TRUE;
    default:
      return FALSE;
  }
}

}  // namespace ftrace
