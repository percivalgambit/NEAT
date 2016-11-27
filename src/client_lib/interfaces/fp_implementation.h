#ifndef CLIENT_LIB_INTERFACES_FP_IMPLEMENTATION_H_
#define CLIENT_LIB_INTERFACES_FP_IMPLEMENTATION_H_

#include <pin.H>

#include "client_lib/utils/fp_operation.h"

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
   * @param[in] operation The operation to perform.
   * @return The result of the floating-point operation.
   */
  virtual FLT32 PerformOperation(const FpOperation &operation);

 protected:
  /**
   * Performs floating-point addition.
   *
   * @param[in] operation The operation to perform.
   * @return The result of the floating-point addition.
   */
  virtual FLT32 FpAdd(const FpOperation &operation) = 0;

  /**
   * Performs floating-point subtraction.
   *
   * @param[in] operation The operation to perform.
   * @return The result of the floating-point subtraction.
   */
  virtual FLT32 FpSub(const FpOperation &operation) = 0;

  /**
   * Performs floating-point multiplication.
   *
   * @param[in] operation The operation to perform.
   * @return The result of the floating-point multiplication.
   */
  virtual FLT32 FpMul(const FpOperation &operation) = 0;

  /**
   * Performs floating-point division.
   *
   * @param[in] operation The operation to perform.
   * @return The result of the floating-point division.
   */
  virtual FLT32 FpDiv(const FpOperation &operation) = 0;
};

}  // namespace ftrace

#endif  // CLIENT_LIB_INTERFACES_FP_IMPLEMENTATION_H_
