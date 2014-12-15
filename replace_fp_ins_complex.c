#include "replace_fp_ins_complex.h"

float replace_fp_ins_complex(float operand1, float operand2, OPCODE operation) {
    switch (operation):
        case XED_ICLASS_ADDSS:
            return (operand1 + operand2) * 0.9;
        case XED_ICLASS_SUBSS:
            return (operand1 - operand2) * 0.9;
        case XED_ICLASS_MULSS:
            return (operand1 * operand2) * 0.9;
        case XED_ICLASS_DIVSS:
            return (operand1 / operand2) * 0.9;
}
