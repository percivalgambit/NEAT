#ifndef CLIENT_REGISTER_INITIALIZED_FP_SELECTOR_H_
#define CLIENT_REGISTER_INITIALIZED_FP_SELECTOR_H_

#include <string>

#include "shared/floating_point_implementation_selector.h"
#include "shared/internal/fp_selector_registry.h"

namespace ftrace {

struct RegisterInitializedFpSelector {
 public:
  RegisterInitializedFpSelector(
      FloatingPointImplementationSelector *fp_selector,
      const string &fp_selector_name) {
    FpSelectorRegistry::GetFpSelectorRegistry()->RegisterFpSelector(
        fp_selector, fp_selector_name);
  }
};

}  // namespace ftrace

#endif  // CLIENT_REGISTER_INITIALIZED_FP_SELECTOR_H_
