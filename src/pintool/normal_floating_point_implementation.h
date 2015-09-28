#ifndef PINTOOL_NORMAL_FLOATING_POINT_IMPLEMENTATION_H_
#define PINTOOL_NORMAL_FLOATING_POINT_IMPLEMENTATION_H_

#include <pin.H>

#include "client/interfaces/floating_point_implementation.h"

namespace ftrace {

class NormalFloatingPointImplementation : public FloatingPointImplementation {
 public:
  FLT32 FloatingPointOperation(const FLT32 &operand1, const FLT32 &operand2,
                               const OPCODE &operation) override;
};

}  // namespace ftrace

#endif  // PINTOOL_NORMAL_FLOATING_POINT_IMPLEMENTATION_H_
