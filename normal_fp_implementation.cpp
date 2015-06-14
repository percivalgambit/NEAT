
/*!
 * @file
 * This is a standard implementation of floating-point arithmetic operations, to
 * be used in the ftrace application and in user-defined replacement operations.
 */

#include "ftrace.h"

/*!
 * Standard implementation of floating-point arithmetic operations.  The result
 * obtained from this function should equal the result of performing an
 * arithmetic operation with no instrumentation.
 * @param[in]   operand1        the first operand of the arithmetic operation
 * @param[in]   operand2        the second operand of the arithmetic operation
 * @param[in]   operation       the arithmetic operation to perform
 * @param[in]   replace_type    extra variable to provide information on which
 *                              replacement strategy to use for this operation
 * @return  the value of the computed operation
 */
FLT32 normal_fp_implementation(FLT32 operand1, FLT32 operand2, OPCODE operation,
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
