#ifndef PINTOOL_INSTRUMENTATION_CALLBACKS_H_
#define PINTOOL_INSTRUMENTATION_CALLBACKS_H_

#include <pin.H>

#include <stack>
#include <string>

#include "pintool/instrumentation_args.h"

namespace ftrace {

/**
 * Run a user-specified function as a replacement for a floating-point
 * arithmetic
 * operation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on two registers, instead of executing the instruction itself.
 * @param[in]   op          the opcode of the floating-point instruction
 * @param[in]   operand1    the first operand of the instruction
 * @param[in]   operand2    the second operand of the instruction
 * @param[in]   ctxt        the context of the instrumented application
 * immediately
 *                          before the instruction is executed
 */
VOID ReplaceRegisterFloatingPointInstruction(
    const InstrumentationArgs *instrumentation_args, const OPCODE operation,
    const REG operand1, const REG operand2, CONTEXT *ctxt);

/**
 * Run a user-specified function as a replacement for a floating-point
 * arithmetic
 * operation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on a register and a memory location, instead of executing the
 * instruction itself.
 * @param[in]   op          the opcode of the floating-point instruction
 * @param[in]   operand1    the first operand of the instruction
 * @param[in]   operand2    the second operand of the instruction
 * @param[in]   ctxt        the context of the instrumented application
 * immediately
 *                          before the instruction is executed
 */
VOID ReplaceMemoryFloatingPointInstruction(
    const InstrumentationArgs *instrumentation_args, const OPCODE operation,
    const REG operand1, const FLT32 *operand2, CONTEXT *ctxt);

/**
 * Push a function-level replacement type on the replacement type stack.  If
 * the replacement type to push is the default replacement type, then nothing
 * will happen
 * @param[in]   replace_type    replacement type to push
 */
VOID FunctionStackPush(const string *function_name,
                       stack<string> *function_stack);

/**
 * Pop a function-level replacement type from the replacement type stack.  A
 * single parameter is supplied, which is the expected replacement type to pop.
 * If the replacement type to pop is the default replacement type, then nothing
 * will happen
 * @param[in]   replace_type    expected replacement type to pop
 */
VOID FunctionStackPop(stack<string> *function_stack);

}  // namespace ftrace

#endif  // PINTOOL_INSTRUMENTATION_CALLBACKS_H_
