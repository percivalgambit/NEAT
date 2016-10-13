#include "pintool/instrument_routine.h"

#include <pin.H>

#include <string>

#include "client_lib/interfaces/fp_selector.h"
#include "pintool/instrumentation_callbacks.h"

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
static BOOL IsFpInstruction(const INS &ins) {
  const OPCODE op = INS_Opcode(ins);
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

namespace ftrace {

VOID ReplaceFpOperations(const RTN rtn, FpSelector *fp_selector) {
  RTN_Open(rtn);
  const string &function_name = RTN_Name(rtn);
  // clang-format off
  RTN_InsertCall(
      rtn, IPOINT_BEFORE,
      reinterpret_cast<AFUNPTR>(EnterFunction),
      IARG_PTR, &function_name,
      IARG_PTR, fp_selector,
      IARG_END);
  // clang-format on

  // Pass through every instruction in the routine and replace every
  // floating-point instruction.
  for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)) {
    if (IsFpInstruction(ins)) {
      REGSET regs_in, regs_out;
      REGSET_Clear(regs_in);
      REGSET_Clear(regs_out);
      REGSET_Insert(regs_in, INS_OperandReg(ins, 0));
      REGSET_Insert(regs_out, INS_OperandReg(ins, 0));

      INS_Delete(ins);
      if (INS_OperandIsReg(ins, 1)) {
        REGSET_Insert(regs_in, INS_OperandReg(ins, 1));
        // clang-format off
        INS_InsertCall(
            ins, IPOINT_BEFORE,
            reinterpret_cast<AFUNPTR>(ReplaceRegisterFpInstruction),
            IARG_UINT32, INS_Opcode(ins),
            IARG_UINT32, INS_OperandReg(ins, 0),
            IARG_UINT32, INS_OperandReg(ins, 1),
            IARG_PTR, fp_selector,
            IARG_PARTIAL_CONTEXT, &regs_in, &regs_out,
            IARG_END);
        // clang-format on
      } else {
        // clang-format off
        INS_InsertCall(
            ins, IPOINT_BEFORE,
            reinterpret_cast<AFUNPTR>(ReplaceMemoryFpInstruction),
            IARG_UINT32, INS_Opcode(ins),
            IARG_UINT32, INS_OperandReg(ins, 0),
            IARG_MEMORYREAD_EA,
            IARG_PTR, fp_selector,
            IARG_PARTIAL_CONTEXT, &regs_in, &regs_out,
            IARG_END);
        // clang-format on
      }
    }
  }
  // clang-format off
  RTN_InsertCall(
      rtn, IPOINT_AFTER,
      reinterpret_cast<AFUNPTR>(ExitFunction),
      IARG_PTR, &function_name,
      IARG_PTR, fp_selector,
      IARG_END);
  // clang-format on
  RTN_Close(rtn);
}

VOID PrintFpOperations(const INS ins, ofstream *output) {
  if (IsFpInstruction(ins)) {
    if (INS_OperandIsReg(ins, 1)) {
      // clang-format off
      INS_InsertCall(
          ins, IPOINT_BEFORE,
          reinterpret_cast<AFUNPTR>(PrintRegisterFpOperands),
          IARG_UINT32, INS_Opcode(ins),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
          IARG_PTR, output,
          IARG_CALL_ORDER, CALL_ORDER_FIRST,
          IARG_END);
      // clang-format on
    } else {
      // clang-format off
      INS_InsertCall(
          ins, IPOINT_BEFORE,
          reinterpret_cast<AFUNPTR>(PrintMemoryFpOperands),
          IARG_UINT32, INS_Opcode(ins),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
          IARG_MEMORYREAD_EA,
          IARG_PTR, output,
          IARG_CALL_ORDER, CALL_ORDER_FIRST,
          IARG_END);
      // clang-format on
    }

    // clang-format off
    INS_InsertCall(
        ins, IPOINT_AFTER,
        reinterpret_cast<AFUNPTR>(PrintFpResult),
        IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
        IARG_PTR, output,
        IARG_END);
    // clang-format on
  }
}

}  // namespace ftrace
