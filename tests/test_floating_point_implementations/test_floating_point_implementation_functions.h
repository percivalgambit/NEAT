#ifndef TEST_FLOATING_POINT_IMLEMENTATION_FUNCTIONS_H_
#define TEST_FLOATING_POINT_IMLEMENTATION_FUNCTIONS_H_

#include <pin.H>

#include "shared/program_state.h"

namespace ftrace {

FLT32 SimpleTestFloatingPointOperation(
    const FLT32 &operand1, const FLT32 &operand2, const OPCODE &operation);

FLT32 ComplexTestFloatingPointOperation(
    const FLT32 &operand1, const FLT32 &operand2, const OPCODE &operation);

}  // namespace ftrace

#endif  // TEST_FLOATING_POINT_IMLEMENTATION_FUNCTIONS_H_
