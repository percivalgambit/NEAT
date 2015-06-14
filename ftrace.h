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

#define FLT_TO_HEX(fp)                                                         \
    StringHex(*(UINT32 *)&fp, 8, FALSE)                                        \

#define WRITE_FLT_OPERANDS(operation, operand1, operand2, output)              \
    do {                                                                       \
        if (output.is_open()) {                                                \
            output << OPCODE_StringShort(operation)                            \
                   << " " << FLT_TO_HEX(operand1)                              \
                   << " " << FLT_TO_HEX(operand2)                              \
                   << "\n";                                                    \
        }                                                                      \
    } while (0)

#define WRITE_FLT_RESULT(result, output)                                       \
    do {                                                                       \
        if (output.is_open()) {                                                \
            output << "  " << FLT_TO_HEX(result) << "\n";                      \
        }                                                                      \
    } while (0)

extern UINT64 ins_count; /*!< count of the total number of instructions in the
                       instrumented program */

extern ofstream OutFile; /*!<  Output file for the pintool */

FLT32 _normal_fp_implementation(FLT32 operand1, FLT32 operand2, OPCODE operation,
                                UINT32 replace_type);

#endif
