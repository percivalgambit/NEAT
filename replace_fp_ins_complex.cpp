
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
 * The floating-point replacement function.
 * @param[in]   operand1    first operand of the floating-point instruction
 * @param[in]   operand2    second operand of the floating-point instruction
 * @param[in]   operation   type of floating-point instruction
 * @return the new result of the floating-point instruction
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
