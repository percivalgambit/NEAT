#ifndef CLIENT_REGISTER_FP_SELECTOR_H_
#define CLIENT_REGISTER_FP_SELECTOR_H_

#include <string>

#include "shared/internal/fp_selector_registry.h"

namespace ftrace {

template <typename FpSelector>
class ReigsterFpSelector {
 public:
  RegisterFpSelector(const string &fp_selector_name) {
    FpSelectorRegistry::GetFpSelectorRegistry()->RegisterFpSelector(
        &fp_selector, fp_selector_name);
  }

 private:
  FpSelector fp_selector;
};

}  // namespace ftrace

#endif  // CLIENT_REGISTER_FP_SELECTOR_H_
