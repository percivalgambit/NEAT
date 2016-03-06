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

/**
 * The name of the symbol which stores the floating-point implementation
 * selector that should be used when instrumenting an application.
 */
#define FLOATING_POINT_IMPL_SELECTOR_NAME floating_point_impl_selector

/**
 * Registers the given floating-point implementation in a library so that the
 * pintool can load it from that library. Only one floating-point
 * implementation can be registered in a single library.
 * @param[in]   floating_point_impl_type   name of the floating-point
 *                                         implementation to register
 */
#define REGISTER_FLOATING_POINT_IMPL_SELECTOR(floating_point_impl_selector_type, ...)  \
  extern "C" floating_point_impl_selector_type FLOATING_POINT_IMPL_SELECTOR_NAME; \
  floating_point_impl_selector_type FLOATING_POINT_IMPL_SELECTOR_NAME = floating_point_impl_selector_type(__VA_ARGS__);

}  // namespace ftrace

#endif  // SHARED_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_
