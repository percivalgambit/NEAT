/**
 * Contains utilities used by multiple instrumentation routines.
 */

#ifndef PINTOOL_UTILS_H_
#define PINTOOL_UTILS_H_

#include <pin.H>

namespace ftrace {

/**
 * Return true if an instruction is an SSE floating-point arithmetic
 * instruction.
 * This function is called on every instruction while the instrumented program
 * is running if the KnobFpSelectorName flag is supplied on the command line.
 *
 * @param[in] ins The instruction to test.
 * @return Whether an instruction is a floating-point arithmetic instruction.
 * @note Currently, the instructions defined as SSE floating-point arithmetic
 *     operations are:
 *       - ADDSS
 *       - SUBSS
 *       - MULSS
 *       - DIVSS
 */
BOOL IsFpInstruction(const INS &ins);

}  // namespace ftrace

#endif  // PINTOOL_UTILS_H_
