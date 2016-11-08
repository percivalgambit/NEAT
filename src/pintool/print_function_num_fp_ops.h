#ifndef PINTOOL_PRINT_FUNCTION_NUM_FP_OPS_H_
#define PINTOOL_PRINT_FUNCTION_NUM_FP_OPS_H_

#include <pin.H>

#include <fstream>

namespace ftrace {

/**
 * Instruments an application with functions to print the number of
 * floating-point arithmetic operations executed per function in the
 * application.
 *
 * @param[in] output The output file to write to.
 */
VOID PrintFunctionNumFpOps(ofstream *output);

}  // namespace ftrace

#endif  // PINTOOL_PRINT_FUNCTION_NUM_FP_OPS_H_
