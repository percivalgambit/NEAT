#ifndef CLIENT_LIB_INTERFACES_FP_IMPLEMENTATION_H_
#define CLIENT_LIB_INTERFACES_FP_IMPLEMENTATION_H_

#include <pin.H>

namespace ftrace {

/**
 * Implementation of floating-point arithmetic to be used in the ftrace
 * instrumentation tool.
 */
class FpImplementation {
 public:
  /**
   * Performs a single floating-point arithmetic operation.
   *
   * @param[in] operand1 First operand of the operation.
   * @param[in] operand2 Second operand of the operation.
   * @param[in] operation The arithmetic operation to perform.
   * @return The result of the floating-point operation.
   */
  FLT32 FpOperation(const FLT32 &operand1, const FLT32 &operand2,
                    const OPCODE &operation);

 protected:
  /**
   * Performs floating-point addition.
   *
   * @param[in] operand1 First operand of the operation.
   * @param[in] operand2 Second operand of the operation.
   * @return The result of the floating-point addition.
   */
  virtual FLT32 FpAdd(const FLT32 &operand1, const FLT32 &operand2) = 0;

  /**
   * Performs floating-point subtraction.
   *
   * @param[in] operand1 First operand of the operation.
   * @param[in] operand2 Second operand of the operation.
   * @return The result of the floating-point subtraction.
   */
  virtual FLT32 FpSub(const FLT32 &operand1, const FLT32 &operand2) = 0;

  /**
   * Performs floating-point multiplication.
   *
   * @param[in] operand1 First operand of the operation.
   * @param[in] operand2 Second operand of the operation.
   * @return The result of the floating-point multiplication.
   */
  virtual FLT32 FpMul(const FLT32 &operand1, const FLT32 &operand2) = 0;

  /**
   * Performs floating-point division.
   *
   * @param[in] operand1 First operand of the operation.
   * @param[in] operand2 Second operand of the operation.
   * @return The result of the floating-point division.
   */
  virtual FLT32 FpDiv(const FLT32 &operand1, const FLT32 &operand2) = 0;
};

}  // namespace ftrace

#endif  // CLIENT_LIB_INTERFACES_FP_IMPLEMENTATION_H_
