
/*! @file
 * This is a more complex replacement function for arithmetic floating-point
 * instructions in the ftrace tool.
 */

#include "ftrace.h"
#include "pin.H"
#include <iostream>

extern UINT64 fp_count;
extern UINT64 instrumented_fp_count;

/*!
 * A more complex implementation of floating-point arithmetic operations.
 * @param[in]   operand1        the first operand of the arithmetic operation
 * @param[in]   operand2        the second operand of the arithmetic operation
 * @param[in]   operation       the arithmetic operation to perform
 * @param[in]   replace_type    extra variable to provide information on which
 *                              replacement strategy to use for this operation
 * @return  the value of the computed operation
 */
FLT32 replace_fp_ins_complex(FLT32 operand1, FLT32 operand2, OPCODE operation,
                             UINT32 replace_type) {

    FLT32 result;

    fp_count++;
    instrumented_fp_count++;

    WRITE_FLT_OPERANDS(operation, operand1, operand2, OutFile);

    switch (operation) {
        case XED_ICLASS_ADDSS:
            result = (operand1 + operand2) * 0.9;
            break;
        case XED_ICLASS_SUBSS:
            result = (operand1 - operand2) * 0.9;
            break;
        case XED_ICLASS_MULSS:
            result = (operand1 * operand2) * 0.9;
            break;
        case XED_ICLASS_DIVSS:
            result = (operand1 / operand2) * 0.9;
            break;
        default:
            std::cerr << "Unexpected opcode " << operation << " encountered "
                         "when replacing floating-point instructions" << endl;
            return operand1;
    }

    WRITE_FLT_RESULT(result, OutFile);
    return result;
}
