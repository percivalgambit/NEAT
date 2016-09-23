#include "shared/internal/fp_selector_registry.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include "shared/floating_point_implementation_selector.h"

namespace ftrace {

extern "C" FpSelectorRegistry FP_SELECTOR_REGISTRY_NAME;
FpSelectorRegistry FP_SELECTOR_REGISTRY_NAME;
FpSelectorRegistry *FpSelectorRegistry::GetFpSelectorRegistry() {
  return &FP_SELECTOR_REGISTRY_NAME;
}

VOID FpSelectorRegistry::RegisterFpSelector(
    FloatingPointImplementationSelector *fp_selector,
    const string &fp_selector_name) {
  if (fp_selector_map_.count(fp_selector_name) > 0) {
    cerr << "Overwriting FpSelectorRegistry entry at " << fp_selector_name
         << endl;
  }
  fp_selector_map_[fp_selector_name] = fp_selector;
}

FloatingPointImplementationSelector *FpSelectorRegistry::GetFpSelectorOrDie(
    const string &fp_selector_name) const {
  if (fp_selector_map_.count(fp_selector_name) == 0) {
    cerr << "No FloatingPointImplementationSelector registered at "
         << fp_selector_name << endl;
    cerr << "Please make sure RegisterFpSelector is used to register your "
            "FloatingPointImplementationSelector."
         << endl;
    exit(1);
  }
  return fp_selector_map_.find(fp_selector_name)->second;
}

}  // namespace ftrace
