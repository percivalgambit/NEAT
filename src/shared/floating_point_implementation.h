#ifndef SHARED_FLOATING_POINT_IMPLEMENTATION_H_
#define SHARED_FLOATING_POINT_IMPLEMENTATION_H_

#include <pin.H>

#include "shared/program_state.h"

namespace ftrace {

/**
 * Implementation of floating-point arithmetic to be used in the ftrace
 * instrumentation tool.
 */
class FloatingPointImplementation {
 public:
  /**
  * Function called before the instrumented application runs.
  */
  virtual VOID StartCallback() {}

  /**
   * Function called after the instrumented application finishes running.
   * @param[in] code OS specific termination code for the application
   */
  virtual VOID ExitCallback(const INT32 &code) {}

  /**
   * Performs floating-point arithmetic.
   * @param[in]  operation    the arithmetic operation to perform
   * @paramm[in] replace_type which replacement type to use
   * @returns the result of the floating-point operation.
   */
  virtual FLT32 FloatingPointOperation(const FLT32 &operand1,
                                       const FLT32 &operand2,
                                       const OPCODE &operation,
                                       const ProgramState &program_state) = 0;
};

/**
 * The name of the symbol which stores the floating-point implementation that
 * should be used when instrumenting an application.
 */
#define FLOATING_POINT_IMPL_NAME floating_point_impl

#define REGISTER_FLOATING_POINT_IMPL(floating_point_impl_type)  \
  extern "C" floating_point_impl_type FLOATING_POINT_IMPL_NAME; \
  floating_point_impl_type FLOATING_POINT_IMPL_NAME

}  // namespace ftrace

#endif  // SHARED_FLOATING_POINT_IMPLEMENTATION_H_
