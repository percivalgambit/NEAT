
/*! @file
 * This is a simple replacement function for arithmetic floating-point instructions
 * in the ftrace tool.
 */

#include "ftrace.h"
#include "pin.H"

extern UINT64 fp_count;
extern UINT64 instrumented_fp_count;

/*!
 * The floating-point replacement function.
 * @param[in]   operand1    first operand of the floating-point instruction
 * @param[in]   operand2    second operand of the floating-point instruction
 * @param[in]   operation   type of floating-point instruction
 * @return the new result of the floating-point instruction
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
