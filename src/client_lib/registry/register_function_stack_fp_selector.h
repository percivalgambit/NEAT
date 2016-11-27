#ifndef CLIENT_LIB_REGISTRY_REGISTER_FUNCTION_STACK_FP_SELECTOR_H_
#define CLIENT_LIB_REGISTRY_REGISTER_FUNCTION_STACK_FP_SELECTOR_H_

#include <pin.H>

#include <stack>
#include <string>
#include <unordered_map>
#include <utility>

#include "client_lib/interfaces/fp_implementation.h"
#include "client_lib/interfaces/fp_selector.h"
#include "client_lib/registry/register_initialized_fp_selector.h"
#include "client_lib/utils/fp_operation.h"

namespace ftrace {
namespace internal {

/**
 * Implementation of a FpSelector that returns a different FpImplementation
 * based on the functions in the instrumented application's call stack. The
 * FpImplementation instance associated with the function name most recent in
 * the call stack will be selected if one exists, otherwise a default
 * FpImplementation will be selected.
 */
class FunctionStackFpSelector : public FpSelector {
 public:
  /**
   * @param[in] function_name_map The map of function names to FpImplementation
   *     instances.
   * @param[in] function_name_map_size The size of function_name_map.
   * @param[in] default_fp_impl The default FpImplementation instance to use.
   */
  FunctionStackFpSelector(
      const pair<string, FpImplementation *> function_name_map[],
      const int function_name_map_size, FpImplementation *default_fp_impl)
      : default_fp_impl_(default_fp_impl) {
    for (int i = 0; i < function_name_map_size; i++) {
      function_name_map_.insert(function_name_map[i]);
    }
  }

  FpImplementation *SelectFpImplementation(
      const FpOperation &operation) override {
    if (fp_impl_stack_.empty()) {
      return default_fp_impl_;
    }
    return fp_impl_stack_.top().second;
  }

  VOID OnFunctionStart(const string &function_name) override {
    if (function_name_map_.count(function_name) > 0) {
      fp_impl_stack_.push(
          make_pair(function_name, function_name_map_[function_name]));
    }
  }

  VOID OnFunctionEnd(const string &function_name) override {
    if (!fp_impl_stack_.empty() &&
        fp_impl_stack_.top().first == function_name) {
      fp_impl_stack_.pop();
    }
  }

 private:
  unordered_map<string, FpImplementation *> function_name_map_;
  FpImplementation *default_fp_impl_;
  /// Keeps track of the current FpImplementation to use based on the current
  /// call stack of the instrumented application.
  stack<pair<const string, FpImplementation *>> fp_impl_stack_;
};

}  // namespace internal

/**
 * Registers an FpSelector that that returns a different FpImplementation
 * based on the functions in the instrumented application's call stack with the
 * global FpSelectorRegistry.
 */
class RegisterFunctionStackFpSelector {
 public:
  /**
   * @param[in] function_name_map The map of function names to FpImplementation
   *     instances.
   * @param[in] function_name_map_size The size of function_name_map.
   * @param[in] default_fp_impl The default FpImplementation instance to use.
   * @param[in] fp_selector_name The name to register for the FpSelector
   *     instance.
   */
  RegisterFunctionStackFpSelector(
      const pair<string, FpImplementation *> function_name_map[],
      const int function_name_map_size, FpImplementation *default_fp_impl,
      const string &fp_selector_name)
      : fp_selector_(function_name_map, function_name_map_size,
                     default_fp_impl),
        register_fp_selector_(&fp_selector_, fp_selector_name) {}

 private:
  internal::FunctionStackFpSelector fp_selector_;
  RegisterInitializedFpSelector register_fp_selector_;
};

}  // namespace ftrace

#endif  // CLIENT_LIB_REGISTRY_REGISTER_FUNCTION_STACK_FP_SELECTOR_H_
