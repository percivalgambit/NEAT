#ifndef PINTOOL_PRINT_FP_BITS_MANIPULATED_H_
#define PINTOOL_PRINT_FP_BITS_MANIPULATED_H_

#include <pin.H>

#include <fstream>

namespace ftrace {

/**
 * Instruments an application with functions to print the number of bits
 * manipulated in every floating-point arithmetic operation in the application.
 *
 * @param[in] output The output file to write to.
 */
VOID PrintFpBitsManipulated(ofstream *output);

}  // namespace ftrace

#endif  // PINTOOL_PRINT_FP_BITS_MANIPULATED_H_
