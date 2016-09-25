#ifndef CLIENT_LIB_REGISTRY_REGISTER_INITIALIZED_FP_SELECTOR_H_
#define CLIENT_LIB_REGISTRY_REGISTER_INITIALIZED_FP_SELECTOR_H_

#include <string>

#include "client_lib/interfaces/floating_point_implementation_selector.h"
#include "client_lib/registry/internal/fp_selector_registry.h"

namespace ftrace {

struct RegisterInitializedFpSelector {
 public:
  RegisterInitializedFpSelector(
      FloatingPointImplementationSelector *fp_selector,
      const string &fp_selector_name) {
    internal::FpSelectorRegistry::GetFpSelectorRegistry()->RegisterFpSelector(
        fp_selector, fp_selector_name);
  }
};

}  // namespace ftrace

#endif  // CLIENT_LIB_REGISTRY_REGISTER_INITIALIZED_FP_SELECTOR_H_
