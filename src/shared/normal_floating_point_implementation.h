#ifndef SHARED_NORMAL_FLOATING_POINT_IMPLEMENTATION_H_
#define SHARED_NORMAL_FLOATING_POINT_IMPLEMENTATION_H_

#include <pin.H>

#include "shared/floating_point_implementation.h"
#include "shared/program_state.h"

namespace ftrace {

class NormalFloatingPointImplementation : public FloatingPointImplementation {
 public:
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation,
                               const ProgramState &program_state) override;
};

}  // namespace ftrace

#endif  // SHARED_NORMAL_FLOATING_POINT_IMPLEMENTATION_H_
