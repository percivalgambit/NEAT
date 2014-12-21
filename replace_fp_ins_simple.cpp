
/*! @file
 * This is a simple replacement function for arithmetic floating-point instructions
 * in the ftrace tool.
 */

#include "pin.H"

/*!
 * The floating-point replacement function.
 * @param[in]   operand1    first operand of the floating-point instruction
 * @param[in]   operand2    second operand of the floating-point instruction
 * @param[in]   operation   type of floating-point instruction
 * @return the new result of the floating-point instruction
 */
FLT32 replace_fp_ins_simple(FLT32 operand1, FLT32 operand2, OPCODE operation) {
    return 1.0;
}
