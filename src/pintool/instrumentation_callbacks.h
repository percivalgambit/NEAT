#ifndef PINTOOL_INSTRUMENTATION_CALLBACKS_H_
#define PINTOOL_INSTRUMENTATION_CALLBACKS_H_

#include <pin.H>

#include <string>
#include <vector>

#include "pintool/instrumentation_args.h"
#include "shared/program_state.h"

namespace ftrace {

/**
 * Performs any setup needed by the given floating-point implementation.
 * This function is called after Pin initialization is finished but before the
 * instrumented application starts running.
 * @param[in]   instrumentation_args     contains the floating-point
 *                                       implementation to setup
 */
VOID StartCallback(const InstrumentationArgs *instrumentation_args);

/**
 * Performs any teardown needed by the given floating-point implementation.
 * This function is called immediately before the instrumented application
 * exits.
 * @param[in]   code                     exit code of the pintool
 * @param[in]   instrumentation_args     contains the floating-point
 *                                       implementation to teardown
 */
VOID ExitCallback(const INT32 code,
                  const InstrumentationArgs *instrumentation_args);

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
    const InstrumentationArgs *instrumentation_args,
    const ProgramState *program_state, const OPCODE operation,
    const REG operand1, const REG operand2, CONTEXT *ctxt);

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
    const InstrumentationArgs *instrumentation_args,
    const ProgramState *program_state, const OPCODE operation,
    const REG operand1, const FLT32 *operand2, CONTEXT *ctxt);

/**
 * Push the name of a function onto the function stack.
 */
VOID FunctionStackPush(const string *function_name,
                       vector<string> *function_stack);

/**
 * Pop the name of a function from the function stack.
 */
VOID FunctionStackPop(vector<string> *function_stack);

}  // namespace ftrace

#endif  // PINTOOL_INSTRUMENTATION_CALLBACKS_H_
