#ifndef CLIENT_REGISTER_SINGLE_FP_IMPLEMENTATION_SELECTOR_H_
#define CLIENT_REGISTER_SINGLE_FP_IMPLEMENTATION_SELECTOR_H_

#include <pin.H>

#include "client/register_fp_selector.h"
#include "shared/floating_point_implementation.h"
#include "shared/floating_point_implementation_selector.h"

namespace ftrace {
namespace internal {

template <typename FpImpl>
class SingleFpImplementationSelector
    : public FloatingPointImplementationSelector {
 public:
  FloatingPointImplementation *SelectFloatingPointImplementation() override {
    return &floating_point_impl_;
  }

 private:
  FpImpl floating_point_impl_;
};

}  // namespace internal

template <typename FpImpl>
class RegisterSingleFpImplementationSelector {
 public:
  RegisterSingleFpImplementationSelector(const string &fp_selector_name)
      : fp_selector_(fp_selector_name) {}

 private:
  RegisterFpSelector<internal::SingleFpImplementationSelector<FpImpl>>
      fp_selector_;
};

}  // namespace ftrace

#endif  // CLIENT_REGISTER_SINGLE_FP_IMPLEMENTATION_SELECTOR_H_
