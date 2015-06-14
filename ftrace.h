#ifndef FTRACE_H
#define FTRACE_H

#include "pin.H"

#include <fstream>
#include <iostream>

#ifdef FUNCTION_LEVEL_REPLACEMENT_TYPE
#include "function_level_replacement_type_enum.h"
#else
#define _no_replacement_type 0
#endif

/*!
 * Convert a FLT32 variable into the string representation of its value in hex.
 * @param[in]   fp  variable to convert to hex
 */
#define FLT_TO_HEX(fp)                                                         \
    StringHex(*(UINT32 *)&fp, 8, FALSE)                                        \

/*!
 * If out is open, write the name of the operation and the value of both
 * operands to out on a single line.
 * @param[in]   operation   operation to write
 * @param[in]   operand1    first operand to write
 * @param[in]   operand2    second operand to write
 * @param[in]   out         stream to write on
 */
#define WRITE_FLT_OPERANDS(operation, operand1, operand2, out)                 \
    do {                                                                       \
        if (out.is_open()) {                                                   \
            out << OPCODE_StringShort(operation)                               \
                << " " << FLT_TO_HEX(operand1)                                 \
                << " " << FLT_TO_HEX(operand2)                                 \
                << "\n";                                                       \
        }                                                                      \
    } while (0)

/*!
 * If out is open, write the value of the result to out on a single line
 * @param[in]   result      result to write
 * @param[in]   out         stream to write on
 */
#define WRITE_FLT_RESULT(result, out)                                          \
    do {                                                                       \
        if (out.is_open()) {                                                   \
            out << "  " << FLT_TO_HEX(result) << "\n";                         \
        }                                                                      \
    } while (0)

extern UINT64 ins_count; /*!< count of the total number of instructions in the
                              instrumented program */

extern ofstream OutFile; /*!<  Output file for the pintool */

FLT32 _normal_fp_implementation(FLT32 operand1, FLT32 operand2, OPCODE operation,
                                UINT32 replace_type);

#endif
