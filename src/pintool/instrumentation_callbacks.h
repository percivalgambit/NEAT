#ifndef PINTOOL_INSTRUMENTATION_CALLBACKS_H_
#define PINTOOL_INSTRUMENTATION_CALLBACKS_H_

#include <pin.H>

#include <string>
#include <vector>

#include "shared/floating_point_implementation_selector.h"

namespace ftrace {

/**
 * Performs any setup needed by the given floating-point implementation.
 * This function is called after Pin initialization is finished but before the
 * instrumented application starts running.
 * @param[in]   instrumentation_args     contains the floating-point
 *                                       implementation to setup
 */
VOID StartCallback(FloatingPointImplementationSelector
                       *floating_point_implementation_selector);

/**
 * Performs any teardown needed by the given floating-point implementation.
 * This function is called immediately before the instrumented application
 * exits.
 * @param[in]   code                     exit code of the pintool
 * @param[in]   instrumentation_args     contains the floating-point
 *                                       implementation to teardown
 */
VOID ExitCallback(const INT32 code, FloatingPointImplementationSelector *
                                        floating_point_implementation_selector);

/**
 * Replaces a floating-point operation with a user defined implementation and
 * prints out the operands and result of the operation if specified on the
 * command line.
 * This function is called for every floating-point arithmetic instruction that
 * operates on two registers.
 * @param[in]   instrumentation_args   contains a user-defined implementation
 *                                     of floating-point operations and
 *                                     invocation-specific information for the
 *                                     pintool
 * @param[in]   program_state          contains information about the current
 *                                     state of the instrumented application
 * @param[in]   operation              opcode of the floating-point operation
 * @param[in]   operand1               first operand of the instruction
 * @param[in]   operand2               second operand of the instruction
 * @param[in]   ctxt                   context of the instrumented application
 *                                     used to store the result of the
 *                                     floating-point operation in the correct
 *                                     register
 */
VOID ReplaceRegisterFloatingPointInstruction(
    const OPCODE operation,
    const REG operand1, const REG operand2,
    FloatingPointImplementationSelector *floating_point_implementation_selector,
    CONTEXT *ctxt);

/**
 * Replaces a floating-point operation with a user defined implementation and
 * prints out the operands and result of the operation if specified on the
 * command line.
 * This function is called for every floating-point arithmetic instruction that
 * operates on a register and a memory location.
 * @param[in]   instrumentation_args   contains a user-defined implementation
 *                                     of floating-point operations and
 *                                     invocation-specific information for the
 *                                     pintool
 * @param[in]   program_state          contains information about the current
 *                                     state of the instrumented application
 * @param[in]   operation              opcode of the floating-point operation
 * @param[in]   operand1               first operand of the instruction
 * @param[in]   operand2               second operand of the instruction
 * @param[in]   ctxt                   context of the instrumented application
 *                                     used to store the result of the
 *                                     floating-point operation in the correct
 *                                     register
 */
VOID ReplaceMemoryFloatingPointInstruction(
    const OPCODE operation,
    const REG operand1, const FLT32 *operand2,
    FloatingPointImplementationSelector *floating_point_implementation_selector,
    CONTEXT *ctxt);

VOID EnterFunction(
    const string *function_name,
    FloatingPointImplementationSelector *floating_point_implementation_selector);

VOID ExitFunction(
    const string *function_name,
    FloatingPointImplementationSelector *floating_point_implementation_selector);

VOID CloseOutputStream(const INT32 code, ofstream *output_stream);

VOID PrintRegisterFPOperands(const OPCODE operation,
                             const PIN_REGISTER *operand1,
                             const PIN_REGISTER *operand2,
                             ofstream *output_stream);

VOID PrintMemoryFPOperands(const OPCODE operation, const PIN_REGISTER *operand1,
                           const FLT32 *operand2, ofstream *output_stream);

VOID PrintFPResult(const PIN_REGISTER *result, ofstream *output_stream);

}  // namespace ftrace

#endif  // PINTOOL_INSTRUMENTATION_CALLBACKS_H_
