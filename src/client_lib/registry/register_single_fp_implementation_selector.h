#ifndef CLIENT_LIB_REGISTRY_REGISTER_SINGLE_FP_IMPLEMENTATION_SELECTOR_H_
#define CLIENT_LIB_REGISTRY_REGISTER_SINGLE_FP_IMPLEMENTATION_SELECTOR_H_

#include <pin.H>

#include <string>

#include "client_lib/interfaces/fp_implementation.h"
#include "client_lib/interfaces/fp_selector.h"
#include "client_lib/registry/register_fp_selector.h"

namespace ftrace {
namespace internal {

/**
 * Implementation of an FpSelector that always returns the same
 * FpImplementation.
 *
 * @tparam FpImpl The FpImplementation class to return.
 */
template <typename FpImpl>
class SingleFpImplementationSelector : public FpSelector {
 public:
  FpImplementation *SelectFpImplementation(
      const FpOperation &operation) override {
    return &fp_impl_;
  }

 private:
  FpImpl fp_impl_;
};

}  // namespace internal

/**
 * Registers a FpSelector instance that always returns the same FpImplementation
 * in the global FpSelectorRegistry.
 *
 * @tparam FpImpl The FpImplementation class to return.
 */
template <typename FpImpl>
class RegisterSingleFpImplementationSelector {
 public:
  /**
   * @param[in] fp_selector_name The name to register for the FpSelector
   *     instance.
   */
  explicit RegisterSingleFpImplementationSelector(
      const string &fp_selector_name)
      : fp_selector_(fp_selector_name) {}

 private:
  RegisterFpSelector<internal::SingleFpImplementationSelector<FpImpl>>
      fp_selector_;
};

}  // namespace ftrace

#endif  // CLIENT_LIB_REGISTRY_REGISTER_SINGLE_FP_IMPLEMENTATION_SELECTOR_H_
