#ifndef CLIENT_SINGLE_FLOATING_POINT_IMPLEMENTATION_GENERATOR_H_
#define CLIENT_SINGLE_FLOATING_POINT_IMPLEMENTATION_GENERATOR_H_

#include "client/program_state.h"
#include "client/interfaces/floating_point_implementation.h"
#include "client/interfaces/floating_point_implementation_generator.h"

namespace ftrace {

template <class FloatingPointImpl>
class SingleFloatingPointImplementationGenerator
    : public FloatingPointImplementationGenerator {
 public:
  FloatingPointImplementation *GenerateFloatingPointImplementation(
      const ProgramState &program_state) override {
    return &floating_point_impl_;
  }

 private:
  FloatingPointImpl floating_point_impl_;
};

}  // namespace ftrace

#endif  // CLIENT_SINGLE_FLOATING_POINT_IMPLEMENTATION_GENERATOR_H_
