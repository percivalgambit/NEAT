#ifndef CLIENT_SINGLE_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_
#define CLIENT_SINGLE_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_

#include <pin.H>

#include "shared/floating_point_implementation.h"
#include "shared/floating_point_implementation_selector.h"

namespace ftrace {

template <class FloatingPointImpl>
class SingleFloatingPointImplementationSelector : public FloatingPointImplementationSelector {
 public:
  FloatingPointImplementation *SelectFloatingPointImplementation() override {
    return &floating_point_impl_;
  }

 private:
  FloatingPointImpl floating_point_impl_;
};

}  // namespace ftrace

#endif  // CLIENT_SINGLE_FLOATING_POINT_IMPLEMENTATION_SELECTOR_H_
