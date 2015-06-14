#include "ftrace.h"

FLT32 _normal_fp_implementation(FLT32 operand1, FLT32 operand2, OPCODE operation,
                                UINT32 replace_type) {
    switch (operation) {
        case XED_ICLASS_ADDSS:
            return operand1 + operand2;
        case XED_ICLASS_SUBSS:
            return operand1 - operand2;
        case XED_ICLASS_MULSS:
            return operand1 * operand2;
        case XED_ICLASS_DIVSS:
            return operand1 / operand2;
        default:
            std::cerr << "Unexpected opcode " << operation << " encountered "
                         "when replacing floating-point instructions" << endl;

            return operand1;
    }
}
