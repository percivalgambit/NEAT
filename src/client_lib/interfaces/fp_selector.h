#ifndef CLIENT_LIB_INTERFACES_FP_SELECTOR_H_
#define CLIENT_LIB_INTERFACES_FP_SELECTOR_H_

#include <pin.H>

#include <string>

#include "client_lib/interfaces/fp_implementation.h"
#include "client_lib/utils/fp_operation.h"

namespace ftrace {

/**
 * Selects which floating-point arithmetic implementation to use for an
 * arithmetic operation depending on the current state of the instrumented
 * application.
 */
class FpSelector {
 public:
  /**
  * Called before the instrumented application runs to perform setup for this
  * class.
  */
  virtual VOID StartCallback() {}

  /**
   * Called after the instrumented application finishes running to perform
   * teardown for this class.
   *
   * @param[in] code OS specific termination code for the application.
   */
  virtual VOID ExitCallback(const INT32 &code) {}

  /**
   * Called whenever a function begins in the instrumented application to
   * perform per-function setup for this class.
   *
   * @param[in] function_name The name of the function that is beginning.
   */
  virtual VOID OnFunctionStart(const string &function_name) {}

  /**
   * Called whenever a function ends in the instrumented application to perform
   * per-function setup for this class.
   *
   * @param[in] function_name The name of the function that is ending.
   */
  virtual VOID OnFunctionEnd(const string &function_name) {}

  /**
   * Selects a floating-point arithmetic implementation to use for the supplied
   * floating-point instruction.
   *
   * @param[in] operation The floating-point instruction to be performed.
   * @return The floating-point implementation to use to calculate the result of
   *     the arithmetic instruction.
   */
  virtual FpImplementation *SelectFpImplementation(
      const FpOperation &operation) = 0;
};

}  // namespace ftrace

#endif  // CLIENT_LIB_INTERFACES_FP_SELECTOR_H_
