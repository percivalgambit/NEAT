#ifndef PINTOOL_PRINT_FP_OPERATIONS_H_
#define PINTOOL_PRINT_FP_OPERATIONS_H_

#include <pin.H>

#include <fstream>

namespace NEAT {

/**
 * Instruments an application with functions to print the operands and result of
 * every floating-point instruction in the instrumented application.
 *
 * @param[in] output The output file to write to.
 */
VOID PrintFpOperations(ofstream *output);

}  // namespace NEAT

#endif  // PINTOOL_PRINT_FP_OPERATIONS_H_
