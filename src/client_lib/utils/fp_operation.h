#ifndef CLIENT_LIB_UTILS_FP_OPERATION_H_
#define CLIENT_LIB_UTILS_FP_OPERATION_H_

#include <pin.H>

#include <string>

namespace ftrace {

/**
 * Contains all the contextual information for a single floating-point
 * arithmetic operation.
 */
struct FpOperation {
  /**
   * @param[in] opcode Opcode of the operation.
   * @param[in] operand1 First operand of the operation.
   * @param[in] operand2 Second operand of the operation.
   * @param[in] function_name Name of the function containing this operation.
   */
  FpOperation(const OPCODE opcode, const FLT32 operand1, const FLT32 operand2,
              const string function_name)
      : opcode(opcode),
        operand1(operand1),
        operand2(operand2),
        function_name(function_name) {}

  OPCODE opcode;
  FLT32 operand1;
  FLT32 operand2;
  string function_name;
};

}  // namespace ftrace

#endif  // CLIENT_LIB_UTILS_FP_OPERATION_H_
