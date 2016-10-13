#ifndef CLIENT_LIB_REGISTRY_REGISTER_INITIALIZED_FP_SELECTOR_H_
#define CLIENT_LIB_REGISTRY_REGISTER_INITIALIZED_FP_SELECTOR_H_

#include <string>

#include "client_lib/interfaces/fp_selector.h"
#include "client_lib/registry/internal/fp_selector_registry.h"

namespace ftrace {

/**
 * Registers a previously allocated FpSelector instance in the global
 * FpSelectorRegistry.
 */
struct RegisterInitializedFpSelector {
 public:
  /**
   * @param[in] fp_selector The FpSelector instance to register.
   * @param[in] fp_selector_name The name to register for the FpSelector
   *     instance.
   */
  RegisterInitializedFpSelector(FpSelector *fp_selector,
                                const string &fp_selector_name) {
    internal::FpSelectorRegistry::GetFpSelectorRegistry()->RegisterFpSelector(
        fp_selector, fp_selector_name);
  }
};

}  // namespace ftrace

#endif  // CLIENT_LIB_REGISTRY_REGISTER_INITIALIZED_FP_SELECTOR_H_
