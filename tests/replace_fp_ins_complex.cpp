
/*! @file
 * This is a more complex replacement function for arithmetic floating-point
 * instructions in the ftrace tool.
 */

#include "pin.H"
#include <iostream>

/*!
 * The floating-point replacement function.
 * @param[in]   operand1    first operand of the floating-point instruction
 * @param[in]   operand2    second operand of the floating-point instruction
 * @param[in]   operation   type of floating-point instruction
 * @return the new result of the floating-point instruction
 */
FLT32 replace_fp_ins_complex(FLT32 operand1, FLT32 operand2, OPCODE operation,
                             UINT32 replace_type) {
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
            std::cerr << "Unexpected opcode " << operation << " encountered "
                         "when replacing floating-point instructions" << endl;
            return operand1;
    }
}
