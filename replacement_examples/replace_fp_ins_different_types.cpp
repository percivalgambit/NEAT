
/*! @file
 * This is a replacement function for arithmetic floating-point instructions that
 * uses the replacement_type parameter to determine which floating-point
 * replacement function to use.
 */

#include "ftrace.h"
#include "pin.H"

FLT32 replace_fp_ins_simple(FLT32, FLT32, OPCODE, UINT32);
FLT32 replace_fp_ins_complex(FLT32, FLT32, OPCODE, UINT32);
FLT32 fp_replacement_callback(FLT32, FLT32, OPCODE, UINT32);

/*!
 * A version of floating-point arithmetic operations that uses replacement_type
 * to differentiate between different implementation options.
 * @param[in]   operand1        the first operand of the arithmetic operation
 * @param[in]   operand2        the second operand of the arithmetic operation
 * @param[in]   operation       the arithmetic operation to perform
 * @param[in]   replace_type    extra variable to provide information on which
 *                              replacement strategy to use for this operation
 * @return  the value of the computed operation
 */
FLT32 replace_fp_ins_different_types(FLT32 operand1, FLT32 operand2,
                                     OPCODE operation, UINT32 replace_type) {
    switch (replace_type) {
        case replacement_simple:
            return replace_fp_ins_simple(operand1, operand2, operation, replace_type);
        case replacement_complex:
            return replace_fp_ins_complex(operand1, operand2, operation, replace_type);
        default:
            return fp_replacement_callback(operand1, operand2, operation, replace_type);
    }
}
