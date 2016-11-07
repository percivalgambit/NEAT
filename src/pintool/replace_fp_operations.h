#ifndef PINTOOL_REPLACE_FP_OPERATIONS_H_
#define PINTOOL_REPLACE_FP_OPERATIONS_H_

#include <pin.H>

#include "client_lib/interfaces/fp_selector.h"

namespace ftrace {

/**
 * Instruments an application with functions from the supplied floating-point
 * selector to replace all floating-point arithmetic operations with
 * user-defined implementations.
 *
 * @param[in,out] fp_selector The floating-point selector.
 */
VOID ReplaceFpOperations(FpSelector *fp_selector);

}  // namespace ftrace

#endif  // PINTOOL_REPLACE_FP_OPERATIONS_H_
