#include <iostream>

#include "replace_fp_ins_complex.h"

FLT32 replace_fp_ins_complex(FLT32 operand1, FLT32 operand2, OPCODE operation) {
    switch (operation) {
        case XED_ICLASS_ADDSS:
            return (operand1 + operand2) * 0.9;
        case XED_ICLASS_SUBSS:
            return (operand1 - operand2) * 0.9;
        case XED_ICLASS_MULSS:
            return (operand1 * operand2) * 0.9;
        case XED_ICLASS_DIVSS:
            return (operand1 / operand2) * 0.9;
        default:
            std::cerr << "Unexpected opcode encountered when replacing"
                         "floating-point instructions" << endl;
            return operand1;
    }
}
