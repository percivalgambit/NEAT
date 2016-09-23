#ifndef SHARED_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_
#define SHARED_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_

#include <pin.H>

#include <string>

#include "shared/floating_point_implementation.h"

namespace ftrace {

/**
 * Selector for the current floating-point arithmetic implementation to use
 * depending on the current state of the instrumented application.
 */
class FloatingPointImplementationSelector {
 public:
  /**
  * Function called before the instrumented application runs.
  */
  virtual VOID StartCallback() {}

  /**
   * Function called after the instrumented application finishes running.
   * @param[in]   code   OS specific termination code for the application
   */
  virtual VOID ExitCallback(const INT32 &code) {}

  virtual VOID OnFunctionStart(const string &function_name) {}

  virtual VOID OnFunctionEnd(const string &function_name) {}

  /**
   * Performs floating-point arithmetic.
   * @param[in]    operation      the arithmetic operation to perform
   * @param[in]   replace_type   which replacement type to use
   * @returns the result of the floating-point operation.
   */
  virtual FloatingPointImplementation *SelectFloatingPointImplementation() = 0;
};

}  // namespace ftrace

#endif  // SHARED_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_
