#ifndef CLIENT_FUNCTION_LEVEL_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_
#define CLIENT_FUNCTION_LEVEL_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_

#include <pin.H>

#include <map>
#include <string>

#include "shared/floating_point_implementation.h"
#include "shared/floating_point_implementation_selector.h"
#include "shared/program_state.h"

namespace ftrace {

class FunctionLevelFloatingPointImplementationSelector : public FloatingPointImplementationSelector {
 public:
  FunctionLevelFloatingPointImplementationSelector(
      const map<string, FloatingPointImplementation> function_name_map,
      FloatingPointImplementation default_floating_point_impl) :
          function_name_map_(function_name_map),
          default_floating_point_impl_(default_floating_point_impl) {}

  FloatingPointImplementation *SelectFloatingPointImplementation(
      const ProgramState &program_state) override {
    for (auto function_name = program_state.function_stack.crbegin();
         function_name != program_state.function_stack.crend();
         ++function_name) {
      if (function_name_map_.find(*function_name) !=
          function_name_map_.end()) {
        return &function_name_map_.at(*function_name);
      }
    }

    return &default_floating_point_impl_
  }

 private:
  const map<string, FloatingPointImplementation> function_name_map_;
  FloatingPointImplementation default_floating_point_impl_;
};

}  // namespace ftrace

#endif  // CLIENT_FUNCTION_LEVEL_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_
