#include <pin.H>

#define FLT_TO_HEX(fp, pin_reg)                                                \
    *pin_reg.flt = fp;                                                         \
    StringHex(*pin_reg.dword, 8, FALSE)                                        \

#define WRITE_FLT_OPERANDS(operation, operand1, operand2, output)              \
    do {                                                                       \
        PIN_REGISTER pin_reg;                                                  \
                                                                               \
        if (output.is_open()) {                                                \
            output << OPCODE_StringShort(operation)                            \
                   << " " << FLT_TO_HEX(operand1, pin_reg)                     \
                   << " " << FLT_TO_HEX(operand2, pin_reg)                     \
                   << "\n";                                                    \
        }                                                                      \
    } while (0)                                                                \

#define WRITE_FLT_RESULT(result, output)                                       \
    do {                                                                       \
        PIN_REGISTER pin_reg;                                                  \
                                                                               \
        if (output.is_open()) {                                                \
            output << "  " << FLT_TO_HEX(result, pin_reg) << "\n";             \
        }                                                                      \
    } while (0)                                                                \

UINT64 ins_count; /*!< count of the total number of instructions in the
                       instrumented program */

ofstream OutFile; /*!<  Output file for the pintool */
