
/*! @file
 * This file contains examples of user-defined functions that are inserted into
 * the code for tests.
 */

#include "ftrace.h"

UINT64 fp_count = 0; /*!< count of the total number of floating point
                          instructions in the instrumented program */

UINT64 instrumented_fp_count = 0; /*!< count of the number of instrumented
                                       floating point instructions in the
                                       instrumented program */

/*!
 * Empty start callback to be used in tests.
 * This function is called before the application starts.
 * @param[in]   v   value specified by the tool in the
 *                  PIN_AddApplicationStartFunction call
 */
VOID start_callback(VOID *v) {
    return;
}

/*!
 * Empty start callback to be used in tests.
 * This function is called when the application exits.
 * @param[in]   code            exit code of the application
 * @param[in]   v               value specified by the tool in the
 *                              PIN_AddFiniFunction function call
 */
VOID exit_callback(INT32 code, VOID *v) {
    if (OutFile.is_open()) {
        OutFile.setf(ios::showbase);
        OutFile << "-----------------------" << endl;
        OutFile << "Total number of instructions: " << ins_count << endl;
        OutFile << "Number of floating point instructions: " << fp_count << endl;
        OutFile << "Number of instrumented floating point instructions: "
                << instrumented_fp_count << endl;
        OutFile.close();
    }
}

/*!
 * Standard implementation of floating-point arithmetic operations to be used in
 * tests. The result obtained from this function should equal the result of
 * performing an arithmetic operation with no instrumentation.
 * @param[in]   operand1        the first operand of the arithmetic operation
 * @param[in]   operand2        the second operand of the arithmetic operation
 * @param[in]   operation       the arithmetic operation to perform
 * @param[in]   replace_type    extra variable to provide information on which
 *                              replacement strategy to use for this operation
 * @return  the value of the computed operation
 */
FLT32 fp_replacement_callback(FLT32 operand1, FLT32 operand2, OPCODE operation,
                              UINT32 replace_type) {
    FLT32 result;

    fp_count++;
    instrumented_fp_count++;

    WRITE_FLT_OPERANDS(operation, operand1, operand2, OutFile);

    switch (operation) {
        case XED_ICLASS_ADDSS:
            result = operand1 + operand2;
            break;
        case XED_ICLASS_SUBSS:
            result = operand1 - operand2;
            break;
        case XED_ICLASS_MULSS:
            result = operand1 * operand2;
            break;
        case XED_ICLASS_DIVSS:
            result = operand1 / operand2;
            break;
        default:
            std::cerr << "Unexpected opcode " << operation << " encountered "
                         "when replacing floating-point instructions" << endl;

            return operand1;
    }

    WRITE_FLT_RESULT(result, OutFile);
    return result;
}
