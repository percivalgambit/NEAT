#ifndef SHARED_INTERNAL_FP_SELECTOR_REGISTRY_H_
#define SHARED_INTERNAL_FP_SELECTOR_REGISTRY_H_

#include <pin.H>

#include <string>
#include <unordered_map>

#include "shared/floating_point_implementation_selector.h"

namespace ftrace {

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

#define FP_SELECTOR_REGISTRY_NAME fp_selector_registry_obj

}  // namespace ftrace

#endif  // SHARED_INTERNAL_FP_SELECTOR_REGISTRY_H_
