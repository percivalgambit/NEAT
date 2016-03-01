#ifndef SHARED_FLOATING_POINT_IMPLEMENTATION_H_
#define SHARED_FLOATING_POINT_IMPLEMENTATION_H_

#include <pin.H>

namespace ftrace {

/**
 * Implementation of floating-point arithmetic to be used in the ftrace
 * instrumentation tool.
 */
class FloatingPointImplementation {
 public:
  /**
   * Performs floating-point arithmetic.
   * @param[in]    operation      the arithmetic operation to perform
   * @param[in]   replace_type   which replacement type to use
   * @returns the result of the floating-point operation.
   */
  virtual FLT32 FloatingPointOperation(const FLT32 &operand1,
                                       const FLT32 &operand2,
                                       const OPCODE &operation) = 0;
};

}  // namespace ftrace

#endif  // SHARED_FLOATING_POINT_IMPLEMENTATION_H_
