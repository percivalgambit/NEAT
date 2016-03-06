#ifndef CLIENT_FUNCTION_LEVEL_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_
#define CLIENT_FUNCTION_LEVEL_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_

#include <pin.H>

#include <map>
#include <stack>
#include <string>

#include "shared/floating_point_implementation.h"
#include "shared/floating_point_implementation_selector.h"

namespace ftrace {

class FunctionLevelFloatingPointImplementationSelector : public FloatingPointImplementationSelector {
 public:
  FunctionLevelFloatingPointImplementationSelector(
      const map<string, FloatingPointImplementation *> function_name_map,
      FloatingPointImplementation *default_floating_point_impl) :
          function_name_map_(function_name_map),
          default_floating_point_impl_(default_floating_point_impl) {}

  FloatingPointImplementation *SelectFloatingPointImplementation() override {
    if (!floating_point_impl_stack_.empty()) {
      return floating_point_impl_stack_.top().second;
    }

    return default_floating_point_impl_;
  }

  VOID OnFunctionStart(const string &function_name) override {
    map<string, FloatingPointImplementation *>::const_iterator floating_point_impl_entry = function_name_map_.find(function_name);
    if (floating_point_impl_entry != function_name_map_.end()) {
      floating_point_impl_stack_.push(*floating_point_impl_entry);
    }
  }

  VOID OnFunctionEnd(const string &function_name) override {
    if (!floating_point_impl_stack_.empty() && floating_point_impl_stack_.top().first == function_name) {
      floating_point_impl_stack_.pop();
    }
  }

 private:
  const map<string, FloatingPointImplementation *> function_name_map_;
  FloatingPointImplementation *default_floating_point_impl_;
  stack<pair<const string, FloatingPointImplementation *>> floating_point_impl_stack_;
};

}  // namespace ftrace

#endif  // CLIENT_FUNCTION_LEVEL_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_
