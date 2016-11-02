/**
 * Contains callbacks to be used for application instrumentation.
 * All of these functions get called while the instrumented application is
 * running unless otherwise stated.
 */

#ifndef PINTOOL_INSTRUMENTATION_CALLBACKS_H_
#define PINTOOL_INSTRUMENTATION_CALLBACKS_H_

#include <pin.H>

#include <string>
#include <vector>

#include "client_lib/interfaces/fp_selector.h"

namespace ftrace {

/**
 * Performs any setup needed by the given floating-point selector.
 * This function is called after Pin initialization is finished but before the
 * instrumented application starts running if the KnobFpSelectorName flag is
 * supplied on the command line.
 *
 * @param[in,out] fp_selector The floating-point selector to setup.
 */
VOID StartCallback(FpSelector *fp_selector);

/**
 * Performs any teardown needed by the given floating-point selector.
 * This function is called immediately before the instrumented application
 * exits if the KnobFpSelectorName flag is supplied on the command line.
 *
 * @param[in] code Exit code of the pintool.
 * @param[in,out] fp_selector The floating-point selector to teardown.
 */
VOID ExitCallback(const INT32 code, FpSelector *fp_selector);

/**
 * Replaces a floating-point operation with a user defined implementation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on two registers if the KnobFpSelectorName flag is supplied on the
 * command line.
 *
 * @param[in] operation Opcode of the floating-point operation.
 * @param[in] operand1 First operand of the instruction.
 * @param[in] operand2 Second operand of the instruction
 * @param[in,out] fp_selector Floating-point selector which selects the
 *     floating-point implementation to use.
 * @param[in,out] ctxt Context of the instrumented application, used to store
 *     the result of the floating-point operation in the correct register.
 */
VOID ReplaceRegisterFpInstruction(const OPCODE operation, const REG operand1,
                                  const REG operand2, FpSelector *fp_selector,
                                  CONTEXT *ctxt);

/**
 * Replaces a floating-point operation with a user defined implementation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on a register and a memory location if the KnobFpSelectorName flag
 * is supplied on the command line.
 *
 * @param[in] operation Opcode of the floating-point operation.
 * @param[in] operand1 First operand of the instruction.
 * @param[in] operand2 Second operand of the instruction
 * @param[in,out] fp_selector Floating-point selector which selects the
 *     floating-point implementation to use.
 * @param[in,out] ctxt Context of the instrumented application, used to store
 *     the result of the floating-point operation in the correct register.
 */
VOID ReplaceMemoryFpInstruction(const OPCODE operation, const REG operand1,
                                const FLT32 *operand2, FpSelector *fp_selector,
                                CONTEXT *ctxt);

/**
 * Performs any per-function setup needed by the given floating-point selector.
 * This function is called every time a new function is entered in the
 * instrumented application if the KnobFpSelectorName flag is supplied on the
 * command line.
 *
 * @param[in] function_name The name of the function being entered.
 * @param[in,out] fp_selector The floating-point selector.
 */
VOID EnterFunction(const string *function_name, FpSelector *fp_selector);

/**
 * Performs any per-function teardown needed by the given floating-point
 * selector.
 * This function is called every time a new function is exited in the
 * instrumented application if the KnobFpSelectorName flag is supplied on the
 * command line.
 *
 * @param[in] function_name The name of the function being exited.
 * @param[in,out] fp_selector The floating-point selector.
 */
VOID ExitFunction(const string *function_name, FpSelector *fp_selector);

/**
 * Closes the supplied output file.
 * This function is called immediately before the instrumented application
 * exits if the KnobPrintFpOps flag is supplied on the command line.
 *
 * @param[in] code Exit code of the pintool.
 * @param[in,out] output The output file to close.
 */
VOID CloseOutputStream(const INT32 code, ofstream *output);

/**
 * Prints the operands of a floating point instruction to the supplied output
 * file.
 * This function is called for every floating-point arithmetic instruction that
 * operates on two registers if the KnobPrintFpOps flag is supplied on the
 * command line.
 *
 * @param[in] operation Opcode of the floating-point operation.
 * @param[in] operand1 First operand of the instruction.
 * @param[in] operand2 Second operand of the instruction
 * @param[out] output The output file to write to.
 * @note All floating-point values are printed as 8 digit hex numbers padded
 *     with 0's.
 * @note Associative operations print the largest operand first so that the
 *     format of the output is identical on different architectures.
 */
VOID PrintRegisterFpOperands(const OPCODE operation,
                             const PIN_REGISTER *operand1,
                             const PIN_REGISTER *operand2, ofstream *output);

/**
 * Prints the operands of a floating point instruction to the supplied output
 * file.
 * This function is called for every floating-point arithmetic instruction that
 * operates on a register and a memory location if the KnobPrintFpOps flag is
 * supplied on the command line.
 *
 * @param[in] operation Opcode of the floating-point operation.
 * @param[in] operand1 First operand of the instruction.
 * @param[in] operand2 Second operand of the instruction
 * @param[out] output The output file to write to.
 * @note All floating-point values are printed as 8 digit hex numbers padded
 *     with 0's.
 * @note Associative operations print the largest operand first so that the
 *     format of the output is identical on different architectures.
 */
VOID PrintMemoryFpOperands(const OPCODE operation, const PIN_REGISTER *operand1,
                           const FLT32 *operand2, ofstream *output);

/**
 * Prints the result of a floating point instruction to the supplied output
 * file.
 * This function is called for every floating-point arithmetic instruction if
 * the KnobPrintFpOps flag is supplied on the command line.
 *
 * @param[in] result Result of the instruction.
 * @param[out] output The output file to write to.
 * @note All floating-point values are printed as 8 digit hex numbers padded
 *     with 0's.
 */
VOID PrintFpResult(const PIN_REGISTER *result, ofstream *output);

}  // namespace ftrace

#endif  // PINTOOL_INSTRUMENTATION_CALLBACKS_H_
