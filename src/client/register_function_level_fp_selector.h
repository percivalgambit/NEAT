#ifndef CLIENT_REGISTER_FUNCTION_LEVEL_FP_SELECTOR_H_
#define CLIENT_REGISTER_FUNCTION_LEVEL_FP_SELECTOR_H_

#include <pin.H>

#include <stack>
#include <string>
#include <utility>

#include "client/register_initialized_fp_selector.h"
#include "shared/floating_point_implementation.h"
#include "shared/floating_point_implementation_selector.h"

namespace ftrace {
namespace internal {

class FunctionLevelFpSelector : public FloatingPointImplementationSelector {
 public:
  FunctionLevelFpSelector(
      const pair<string, FloatingPointImplementation *> function_name_map[],
      const int function_name_map_size,
      FloatingPointImplementation *default_fp_impl)
      : function_name_map_(function_name_map),
        function_name_map_size_(function_name_map_size),
        default_fp_impl_(default_fp_impl) {}

  FloatingPointImplementation *SelectFloatingPointImplementation() override {
    if (fp_impl_stack_.empty()) {
      return default_fp_impl_;
    }
    return fp_impl_stack_.top().second;
  }

  VOID OnFunctionStart(const string &function_name) override {
    for (int i = 0; i < function_name_map_size_; i++) {
      if (function_name_map_[i].first == function_name) {
        fp_impl_stack_.push(function_name_map_[i]);
      }
    }
  }

  VOID OnFunctionEnd(const string &function_name) override {
    if (!fp_impl_stack_.empty() &&
        fp_impl_stack_.top().first == function_name) {
      fp_impl_stack_.pop();
    }
  }

 private:
  const pair<string, FloatingPointImplementation *> *function_name_map_;
  const int function_name_map_size_;
  FloatingPointImplementation *default_fp_impl_;
  stack<pair<const string, FloatingPointImplementation *>> fp_impl_stack_;
};

}  // namespace internal

class RegisterFunctionLevelFpSelector {
 public:
  RegisterFunctionLevelFpSelector(
      const pair<string, FloatingPointImplementation *> function_name_map[],
      const int function_name_map_size,
      FloatingPointImplementation *default_fp_impl,
      const string &fp_selector_name)
      : fp_selector_(function_name_map, function_name_map_size,
                     default_fp_impl),
        register_fp_selector_(&fp_selector_, fp_selector_name) {}

 private:
  internal::FunctionLevelFpSelector fp_selector_;
  RegisterInitializedFpSelector register_fp_selector_;
};

}  // namespace ftrace

#endif  // CLIENT_REGISTER_FUNCTION_LEVEL_FP_SELECTOR_H_
