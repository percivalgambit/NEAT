#include "pintool/instrument_routine.h"

#include <pin.H>

#include <string>

#include "pintool/instrumentation_args.h"
#include "pintool/instrumentation_callbacks.h"
#include "shared/program_state.h"

namespace ftrace {

static ProgramState program_state;

/**
 * Return true if an instruction is an SSE floating-point arithmetic
 * instruction.
 * This function is called on every instruction.
 * @param[in]   ins     the instruction to test
 * @note Currently, the instructions defined as SSE floating-point arithmetic
 *       operations are:
 *  - ADDSS
 *  - SUBSS
 *  - MULSS
 *  - DIVSS
 */
BOOL IsFpInstruction(const INS &ins) {
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

VOID InstrumentRoutine(const RTN rtn,
                       const InstrumentationArgs *instrumentation_args) {
  RTN_Open(rtn);
  const string &function_name = RTN_Name(rtn);
  RTN_InsertCall(rtn, IPOINT_BEFORE,
                 reinterpret_cast<AFUNPTR>(FunctionStackPush),
                 IARG_PTR, &function_name,
                 IARG_PTR, &program_state.function_stack_,
                 IARG_END);

  // Forward pass over all instructions in routine
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
        INS_InsertCall(
            ins, IPOINT_BEFORE,
            reinterpret_cast<AFUNPTR>(ReplaceRegisterFloatingPointInstruction),
            IARG_PTR, instrumentation_args,
            IARG_PTR, &program_state,
            IARG_UINT32, INS_Opcode(ins),
            IARG_UINT32, INS_OperandReg(ins, 0),
            IARG_UINT32, INS_OperandReg(ins, 1),
            IARG_PARTIAL_CONTEXT, &regs_in, &regs_out,
            IARG_END);
      } else {
        INS_InsertCall(
            ins, IPOINT_BEFORE,
            reinterpret_cast<AFUNPTR>(ReplaceMemoryFloatingPointInstruction),
            IARG_PTR, instrumentation_args,
            IARG_PTR, &program_state,
            IARG_UINT32, INS_Opcode(ins),
            IARG_UINT32, INS_OperandReg(ins, 0),
            IARG_MEMORYREAD_EA,
            IARG_PARTIAL_CONTEXT, &regs_in, &regs_out,
            IARG_END);
      }
    }
  }
  // Pop replacement type from stack
  RTN_InsertCall(rtn, IPOINT_AFTER,
                 reinterpret_cast<AFUNPTR>(FunctionStackPop),
                 IARG_PTR, &program_state.function_stack_,
                 IARG_END);
  RTN_Close(rtn);
}

}  // namespace ftrace
