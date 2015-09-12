#include "pintool/ftrace.h"

#include <iostream>
#include <stack>
#include <string>

#include "client/floating_point_implementation.h"

namespace ftrace {

/* ================================================================== */
// Global variables
/* ================================================================== */

stack<string> function_stack;

/* ===================================================================== */
// Utilities
/* ===================================================================== */

/* ===================================================================== */
// Analysis routines
/* ===================================================================== */


VOID PrintFloatingPointOperation(
    const OPCODE operation, const FLT32 operand1, const FLT32 operand2,
    const FLT32 result) {
  cout << OPCODE_StringShort(operation) << " "
       << FLT32_TO_HEX(operand1) << " " << FLT32_TO_HEX(operand2) << endl;
  cout << "  " << FLT32_TO_HEX(result) << endl;
}

/**
 * Run a user-specified function as a replacement for a floating-point arithmetic
 * operation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on two registers, instead of executing the instruction itself.
 * @param[in]   op          the opcode of the floating-point instruction
 * @param[in]   operand1    the first operand of the instruction
 * @param[in]   operand2    the second operand of the instruction
 * @param[in]   ctxt        the context of the instrumented application immediately
 *                          before the instruction is executed
 */
VOID ReplaceRegisterFloatingPointInstruction(
    const RoutineInstrumentationArgs *instrumentation_args,
    const OPCODE operation, const REG operand1, const REG operand2,
    CONTEXT *ctxt) {
  PIN_REGISTER reg1, reg2, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);
  PIN_GetContextRegval(ctxt, operand2, reg2.byte);
  FloatingPointImplementation *floating_point_impl =
      instrumentation_args->floating_point_impl_;

  if (floating_point_impl != nullptr) {
    *result.flt = floating_point_impl->FloatingPointOperation(
        *reg1.flt, *reg2.flt, operation, 0);
    PIN_SetContextRegval(ctxt, operand1, result.byte);
  }
  if (instrumentation_args->print_floating_point_ops_) {
    PrintFloatingPointOperation(operation, *reg1.flt, *reg2.flt, *result.flt);
  }
}

/**
 * Run a user-specified function as a replacement for a floating-point arithmetic
 * operation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on a register and a memory location, instead of executing the
 * instruction itself.
 * @param[in]   op          the opcode of the floating-point instruction
 * @param[in]   operand1    the first operand of the instruction
 * @param[in]   operand2    the second operand of the instruction
 * @param[in]   ctxt        the context of the instrumented application immediately
 *                          before the instruction is executed
 */
VOID ReplaceMemoryFloatingPointInstruction(
    const RoutineInstrumentationArgs *instrumentation_args,
    const OPCODE operation, const REG operand1, const FLT32 *operand2,
    CONTEXT *ctxt) {
  PIN_REGISTER reg1, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);
  FloatingPointImplementation *floating_point_impl =
      instrumentation_args->floating_point_impl_;

  if (floating_point_impl != nullptr) {
    *result.flt = floating_point_impl->FloatingPointOperation(
        *reg1.flt, *operand2, operation, 0);
    PIN_SetContextRegval(ctxt, operand1, result.byte);
  }
  if (instrumentation_args->print_floating_point_ops_) {
    PrintFloatingPointOperation(operation, *reg1.flt, *operand2, *result.flt);
  }
}

/**
 * Push a function-level replacement type on the replacement type stack.  If
 * the replacement type to push is the default replacement type, then nothing
 * will happen
 * @param[in]   replace_type    replacement type to push
 */
VOID FunctionStackPush(const string *function_name) {
  function_stack.push(*function_name);
}

/**
 * Pop a function-level replacement type from the replacement type stack.  A
 * single parameter is supplied, which is the expected replacement type to pop.
 * If the replacement type to pop is the default replacement type, then nothing
 * will happen
 * @param[in]   replace_type    expected replacement type to pop
 */
VOID FunctionStackPop() {
  function_stack.pop();
}

/* ===================================================================== */
// Instrumentation callbacks
/* ===================================================================== */

/**
 * Return true if an instruction is an SSE floating-point arithmetic instruction.
 * This function is called on every instruction.
 * @param[in]   ins     the instruction to test
 * @return true if the instruction is an SSE floating-point arithmetic instruction,
 *         false if not
 * @note Currently, the instructions defined as SSE floating-point arithmetic
 *       operations are:
 *  - ADDSS
 *  - SUBSS
 *  - MULSS
 *  - DIVSS
 */
BOOL IsFpInstruction(const INS &ins) {
    OPCODE op = INS_Opcode(ins);
    switch (op) {
        case XED_ICLASS_ADDSS:
        case XED_ICLASS_SUBSS:
        case XED_ICLASS_MULSS:
        case XED_ICLASS_DIVSS:
            return TRUE;
        default:
            return FALSE;
    }
}

VOID Routine(const RTN rtn, VOID *routine_instrumentation_args) {
  const RoutineInstrumentationArgs *instrumentation_args =
      reinterpret_cast<const RoutineInstrumentationArgs *>(
          routine_instrumentation_args);
  RTN_Open(rtn);
  // Push function-level replacement type on stack
  RTN_InsertCall(rtn,
                 IPOINT_BEFORE,
                 (AFUNPTR)FunctionStackPush,
                 IARG_PTR,
                 &RTN_Name(rtn),
                 IARG_END);

  // Forward pass over all instructions in routine
  for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)) {
    if (IsFpInstruction(ins)) {
      REGSET regs_in, regs_out;
      REGSET_Clear(regs_in);
      REGSET_Clear(regs_out);
      REGSET_Insert(regs_in, (REG)INS_OperandReg(ins, 0));
      REGSET_Insert(regs_out, (REG)INS_OperandReg(ins, 0));

      if (instrumentation_args->floating_point_impl_ != nullptr) {
        INS_Delete(ins);
      }
      if (INS_OperandIsReg(ins, 1)) {
        REGSET_Insert(regs_in, (REG)INS_OperandReg(ins, 1));
        INS_InsertCall(ins,
                       IPOINT_BEFORE,
                       (AFUNPTR)ReplaceRegisterFloatingPointInstruction,
                       IARG_PTR,
                       instrumentation_args,
                       IARG_UINT32,
                       INS_Opcode(ins),
                       IARG_UINT32,
                       INS_OperandReg(ins, 0),
                       IARG_UINT32,
                       INS_OperandReg(ins, 1),
                       IARG_PARTIAL_CONTEXT,
                       &regs_in,
                       &regs_out,
                       IARG_END);
      } else {
        INS_InsertCall(ins,
                       IPOINT_BEFORE,
                       (AFUNPTR)ReplaceMemoryFloatingPointInstruction,
                       IARG_PTR,
                       instrumentation_args,
                       IARG_UINT32,
                       INS_Opcode(ins),
                       IARG_UINT32,
                       INS_OperandReg(ins, 0),
                       IARG_MEMORYREAD_EA,
                       IARG_PARTIAL_CONTEXT,
                       &regs_in,
                       &regs_out,
                       IARG_END);
      }
    }
  }
  // Pop replacement type from stack
  RTN_InsertCall(rtn, IPOINT_AFTER, (AFUNPTR)FunctionStackPop, IARG_END);
  RTN_Close(rtn);
}

}  // namespace ftrace
