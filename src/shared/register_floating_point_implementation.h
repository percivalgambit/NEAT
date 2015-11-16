#ifndef SHARED_REGISTER_FLOATING_POINT_IMPLEMENTATION_H_
#define SHARED_REGISTER_FLOATING_POINT_IMPLEMENTATION_H_

namespace ftrace {

/**
 * The name of the symbol which stores the floating-point implementation that
 * should be used when instrumenting an application.
 */
#define FLOATING_POINT_IMPL_NAME floating_point_impl

#define REGISTER_FLOATING_POINT_IMPL(floating_point_impl_type)                \
  extern "C" floating_point_impl_type FLOATING_POINT_IMPL_NAME;               \
  floating_point_impl_type FLOATING_POINT_IMPL_NAME

}  // namespace ftrace

#endif  // SHARED_REGISTER_FLOATING_POINT_IMPLEMENTATION_H_
