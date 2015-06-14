
/*! @file
 * This is a simple replacement function for arithmetic floating-point instructions
 * in the ftrace tool.
 */

#include "ftrace.h"
#include "pin.H"

extern UINT64 fp_count;
extern UINT64 instrumented_fp_count;

/*!
 * A simple implementation of floating-point arithmetic operations.
 * @param[in]   operand1        the first operand of the arithmetic operation
 * @param[in]   operand2        the second operand of the arithmetic operation
 * @param[in]   operation       the arithmetic operation to perform
 * @param[in]   replace_type    extra variable to provide information on which
 *                              replacement strategy to use for this operation
 * @return  the value of the computed operation
 */
FLT32 replace_fp_ins_simple(FLT32 operand1, FLT32 operand2, OPCODE operation,
                            UINT32 replace_type) {

    FLT32 result = 1.0;

    fp_count++;
    instrumented_fp_count++;

    WRITE_FLT_OPERANDS(operation, operand1, operand2, OutFile);
    WRITE_FLT_RESULT(result, OutFile);
    return result;
}
