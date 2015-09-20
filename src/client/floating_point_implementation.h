#ifndef CLIENT_FLOATING_POINT_IMPLEMENTATION_H_
#define CLIENT_FLOATING_POINT_IMPLEMENTATION_H_

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
   * @param[in]  operation    the arithmetic operation to perform
   * @paramm[in] replace_type which replacement type to use
   * @returns the result of the floating-point operation.
   */
  virtual FLT32 FloatingPointOperation(const FLT32 &operand1,
                                       const FLT32 &operand2,
                                       const OPCODE &operation) = 0;

  /**
   * Function called before the instrumented application runs.
   */
  virtual VOID StartCallback() {}

  /**
   * Function called after the instrumented application finishes running.
   * @param[in] code OS specific termination code for the application
   */
  virtual VOID ExitCallback(const INT32 &code) {}
};

/**
 * The name of the symbol which stores the floating-point implementation that
 * should be used when instrumenting an application.
 */
#define FLOATING_POINT_IMPL_NAME "floating_point_impl"

/**
 * Registers a floating-point implementation to be used when instrumenting an
 * application.
 */
#define REGISTER_FLOATING_POINT_IMPL(floating_point_impl_type) \
  extern "C" floating_point_impl_type floating_point_impl;     \
  floating_point_impl_type floating_point_impl

}  // namespace ftrace

#endif  // CLIENT_FLOATING_POINT_IMPLEMENTATION_H_
