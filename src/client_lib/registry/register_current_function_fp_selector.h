#ifndef CLIENT_LIB_REGISTRY_REGISTER_CURRENT_FUNCTION_FP_SELECTOR_H_
#define CLIENT_LIB_REGISTRY_REGISTER_CURRENT_FUNCTION_FP_SELECTOR_H_

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
 * based on the current function in the instrumented application. The
 * FpImplementation instance associated with the current function will be
 * selected, otherwise a default FpImplementation will be selected.
 */
class CurrentFunctionFpSelector : public FpSelector {
 public:
  /**
   * @param[in] function_name_map The map of function names to FpImplementation
   *     instances.
   * @param[in] function_name_map_size The size of function_name_map.
   * @param[in] default_fp_impl The default FpImplementation instance to use.
   */
  CurrentFunctionFpSelector(
      const pair<string, FpImplementation *> function_name_map[],
      const int function_name_map_size, FpImplementation *default_fp_impl)
      : default_fp_impl_(default_fp_impl) {
    for (int i = 0; i < function_name_map_size; i++) {
      function_name_map_.insert(function_name_map[i]);
    }
  }

  FpImplementation *SelectFpImplementation(
      const FpOperation &operation) override {
    if (function_name_map_.count(operation.function_name) > 0) {
      return function_name_map_[operation.function_name];
    }
    return default_fp_impl_;
  }

 private:
  unordered_map<string, FpImplementation *> function_name_map_;
  FpImplementation *default_fp_impl_;
};

}  // namespace internal

/**
 * Registers an FpSelector that that returns a different FpImplementation
 * based on the functions in the instrumented application's call stack with the
 * global FpSelectorRegistry.
 */
class RegisterCurrentFunctionFpSelector {
 public:
  /**
   * @param[in] function_name_map The map of function names to FpImplementation
   *     instances.
   * @param[in] function_name_map_size The size of function_name_map.
   * @param[in] default_fp_impl The default FpImplementation instance to use.
   * @param[in] fp_selector_name The name to register for the FpSelector
   *     instance.
   */
  RegisterCurrentFunctionFpSelector(
      const pair<string, FpImplementation *> function_name_map[],
      const int function_name_map_size, FpImplementation *default_fp_impl,
      const string &fp_selector_name)
      : fp_selector_(function_name_map, function_name_map_size,
                     default_fp_impl),
        register_fp_selector_(&fp_selector_, fp_selector_name) {}

 private:
  internal::CurrentFunctionFpSelector fp_selector_;
  RegisterInitializedFpSelector register_fp_selector_;
};

}  // namespace ftrace

#endif  // CLIENT_LIB_REGISTRY_REGISTER_CURRENT_FUNCTION_FP_SELECTOR_H_
