#ifndef CLIENT_INTERFACES_FLOATING_POINT_IMPLEMENTATION_GENERATOR_H_
#define CLIENT_INTERFACES_FLOATING_POINT_IMPLEMENTATION_GENERATOR_H_

#include <pin.H>

#include "client/program_state.h"
#include "client/interfaces/floating_point_implementation.h"

namespace ftrace {

class FloatingPointImplementationGenerator {
 public:
  virtual FloatingPointImplementation *GenerateFloatingPointImplementation(
      const ProgramState &program_state) = 0;

  /**
   * Function called before the instrumented application runs.
   */
  virtual VOID StartCallback() {}

  /**
   * Function called after the instrumented application finishes running.
   * @param[in] code OS specific termination code for the application
   */
  virtual VOID ExitCallback(const INT32 &code) {}
};

}  // namespace ftrace

#endif  // CLIENT_INTERFACES_FLOATING_POINT_IMPLEMENTATION_GENERATOR_H_
