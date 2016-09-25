#ifndef CLIENT_LIB_REGISTRY_REGISTER_FP_SELECTOR_H_
#define CLIENT_LIB_REGISTRY_REGISTER_FP_SELECTOR_H_

#include <string>

#include "client_lib/registry/internal/fp_selector_registry.h"

namespace ftrace {

template <typename FpSelector>
struct RegisterFpSelector {
 public:
  explicit RegisterFpSelector(const string &fp_selector_name) {
    internal::FpSelectorRegistry::GetFpSelectorRegistry()->RegisterFpSelector(
        &fp_selector, fp_selector_name);
  }

 private:
  FpSelector fp_selector;
};

}  // namespace ftrace

#endif  // CLIENT_LIB_REGISTRY_REGISTER_FP_SELECTOR_H_
