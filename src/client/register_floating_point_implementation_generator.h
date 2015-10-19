#ifndef CLIENT_REGISTER_FLOATING_POINT_IMPLEMENTATION_GENERATOR_H_
#define CLIENT_REGISTER_FLOATING_POINT_IMPLEMENTATION_GENERATOR_H_

#include "client/single_floating_point_implementation_generator.h"

namespace ftrace {

/**
 * The name of the symbol which stores the floating-point implementation that
 * should be used when instrumenting an application.
 */
#define FPIG_NAME fpig

#define REGISTER_FPIG(fpig_type)                                              \
  extern "C" fpig_type FPIG_NAME;                                             \
  fpig_type FPIG_NAME

/**
 * Registers a floating-point implementation to be used when instrumenting an
 * application.
 */
#define REGISTER_SINGLE_FLOATING_POINT_IMPL(floating_point_impl_type)         \
  REGISTER_FPIG(SingleFloatingPointImplementationGenerator<floating_point_impl_type>);

}  // namespace ftrace

#endif  // CLIENT_REGISTER_FLOATING_POINT_IMPLEMENTATION_GENERATOR_H_
