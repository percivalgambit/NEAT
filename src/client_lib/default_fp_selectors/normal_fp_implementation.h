#ifndef CLIENT_LIB_DEFAULT_FP_SELECTORS_NORMAL_FP_IMPLEMENTATION_H_
#define CLIENT_LIB_DEFAULT_FP_SELECTORS_NORMAL_FP_IMPLEMENTATION_H_

#include <pin.H>

#include <iostream>

#include "client_lib/interfaces/fp_implementation.h"
#include "client_lib/registry/register_fp_selector.h"

namespace ftrace {

/**
 * A default implementation of floating-point arithmetic that performs each
 * operation normally.
 */
class NormalFpImplementation : public FpImplementation {
 public:
  FLT32 FpAdd(const FLT32 &operand1, const FLT32 &operand2) {
    return operand1 + operand2;
  }

  FLT32 FpSub(const FLT32 &operand1, const FLT32 &operand2) {
    return operand1 - operand2;
  }

  FLT32 FpMul(const FLT32 &operand1, const FLT32 &operand2) {
    return operand1 * operand2;
  }

  FLT32 FpDiv(const FLT32 &operand1, const FLT32 &operand2) {
    return operand1 / operand2;
  }
};

}  // namespace ftrace

#endif  // CLIENT_LIB_DEFAULT_FP_SELECTORS_NORMAL_FP_IMPLEMENTATION_H_
