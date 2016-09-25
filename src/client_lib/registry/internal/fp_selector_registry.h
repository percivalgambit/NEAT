#ifndef CLIENT_LIB_REGISTRY_INTERNAL_FP_SELECTOR_REGISTRY_H_
#define CLIENT_LIB_REGISTRY_INTERNAL_FP_SELECTOR_REGISTRY_H_

#include <pin.H>

#include <string>
#include <unordered_map>

#include "client_lib/interfaces/floating_point_implementation_selector.h"

namespace ftrace {
namespace internal {

class FpSelectorRegistry {
 public:
  static FpSelectorRegistry *GetFpSelectorRegistry();

  VOID RegisterFpSelector(FloatingPointImplementationSelector *fp_selector,
                          const string &fp_selector_name);

  FloatingPointImplementationSelector *GetFpSelectorOrDie(
      const string &fp_selector_name) const;

 private:
  unordered_map<string, FloatingPointImplementationSelector *> fp_selector_map_;
};

}  // namespace internal
}  // namespace ftrace

#endif  // CLIENT_LIB_REGISTRY_INTERNAL_FP_SELECTOR_REGISTRY_H_
