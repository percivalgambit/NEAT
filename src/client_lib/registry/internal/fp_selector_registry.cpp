#include "client_lib/registry/internal/fp_selector_registry.h"

#include <pin.H>

#include <cstdlib>
#include <iostream>
#include <string>

#include "client_lib/interfaces/floating_point_implementation_selector.h"

namespace ftrace {
namespace internal {

static FpSelectorRegistry fp_selector_registry_obj;
FpSelectorRegistry *FpSelectorRegistry::GetFpSelectorRegistry() {
  return &fp_selector_registry_obj;
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

}  // namespace internal
}  // namespace ftrace
