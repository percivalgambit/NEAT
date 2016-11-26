#ifndef CLIENT_LIB_DEFAULT_FP_SELECTORS_NORMAL_FP_IMPLEMENTATION_H_
#define CLIENT_LIB_DEFAULT_FP_SELECTORS_NORMAL_FP_IMPLEMENTATION_H_

#include <pin.H>

#include <iostream>

#include "client_lib/interfaces/fp_implementation.h"
#include "client_lib/registry/register_fp_selector.h"
#include "client_lib/utils/fp_operation.h"

namespace ftrace {

/**
 * A default implementation of floating-point arithmetic that performs each
 * operation normally.
 */
class NormalFpImplementation : public FpImplementation {
 public:
  FLT32 FpAdd(const FpOperation &operation) override {
    return operation.operand1 + operation.operand2;
  }

  FLT32 FpSub(const FpOperation &operation) override {
    return operation.operand1 - operation.operand2;
  }

  FLT32 FpMul(const FpOperation &operation) override {
    return operation.operand1 * operation.operand2;
  }

  FLT32 FpDiv(const FpOperation &operation) override {
    return operation.operand1 / operation.operand2;
  }
};

}  // namespace ftrace

#endif  // CLIENT_LIB_DEFAULT_FP_SELECTORS_NORMAL_FP_IMPLEMENTATION_H_
