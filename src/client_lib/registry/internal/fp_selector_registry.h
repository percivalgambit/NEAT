#ifndef CLIENT_LIB_REGISTRY_INTERNAL_FP_SELECTOR_REGISTRY_H_
#define CLIENT_LIB_REGISTRY_INTERNAL_FP_SELECTOR_REGISTRY_H_

#include <pin.H>

#include <string>
#include <unordered_map>

#include "client_lib/interfaces/fp_selector.h"

namespace ftrace {
namespace internal {

/**
 * Contains a mapping from names to FpSelector instances. This is used to
 * determine which FpSelector instance to use to instrument a program when the
 * KnobFpSelectorName flag is supplied on the command line.
 */
class FpSelectorRegistry {
 public:
  /**
   * Returns the global registry for FpSelector instances.
   */
  static FpSelectorRegistry *GetFpSelectorRegistry();

  /**
   * Creates a new mapping from a name to an FpSelector instance in the
   * registry.
   *
   * @param[in] fp_selector The FpSelector instance to register.
   * @param[in] fp_selector_name The name to register for the FpSelector
   *     instance.
   */
  VOID RegisterFpSelector(FpSelector *fp_selector,
                          const string &fp_selector_name);

  /**
   * Returns the FpSelector instance mapped to the supplied name, or exits the
   * application if no instance is mapped to that name.
   *
   * @param[in] fp_selector_name The name to look up in the registry.
   * @return The FpSelector instance mapped to the supplied name.
   */
  FpSelector *GetFpSelectorOrDie(const string &fp_selector_name) const;

 private:
  /// Mapping from names to FpSelector instances.
  unordered_map<string, FpSelector *> fp_selector_map_;
};

}  // namespace internal
}  // namespace ftrace

#endif  // CLIENT_LIB_REGISTRY_INTERNAL_FP_SELECTOR_REGISTRY_H_
