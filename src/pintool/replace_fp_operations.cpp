#include "pintool/replace_fp_operations.h"

#include <pin.H>

#include <string>

#include "client_lib/interfaces/fp_implementation.h"
#include "client_lib/interfaces/fp_selector.h"
#include "client_lib/utils/fp_operation.h"
#include "pintool/utils.h"

namespace ftrace {
namespace analysis {
namespace {

/**
 * Replaces a floating-point operation with a user defined implementation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on two registers if the KnobFpSelectorName flag is supplied on the
 * command line.
 *
 * @param[in] opcode Opcode of the floating-point operation.
 * @param[in] operand1 First operand of the instruction.
 * @param[in] operand2 Second operand of the instruction.
 * @param[in] function_name Name of the function containing this operation.
 * @param[in,out] fp_selector Floating-point selector which selects the
 *     floating-point implementation to use.
 * @param[in,out] ctxt Context of the instrumented application, used to store
 *     the result of the floating-point operation in the correct register.
 */
VOID ReplaceRegisterFpInstruction(const OPCODE opcode, const REG operand1,
                                  const REG operand2,
                                  const string *function_name,
                                  FpSelector *fp_selector, CONTEXT *ctxt) {
  PIN_REGISTER reg1, reg2, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);
  PIN_GetContextRegval(ctxt, operand2, reg2.byte);

  FpOperation operation(opcode, *reg1.flt, *reg2.flt, *function_name);
  FpImplementation *fp_implementation =
      fp_selector->SelectFpImplementation(operation);
  *result.flt = fp_implementation->PerformOperation(operation);
  PIN_SetContextRegval(ctxt, operand1, result.byte);
}

/**
 * Replaces a floating-point operation with a user defined implementation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on a register and a memory location if the KnobFpSelectorName flag
 * is supplied on the command line.
 *
 * @param[in] opcode Opcode of the floating-point operation.
 * @param[in] operand1 First operand of the instruction.
 * @param[in] operand2 Second operand of the instruction
 * @param[in] function_name Name of the function containing this operation.
 * @param[in,out] fp_selector Floating-point selector which selects the
 *     floating-point implementation to use.
 * @param[in,out] ctxt Context of the instrumented application, used to store
 *     the result of the floating-point operation in the correct register.
 */
VOID ReplaceMemoryFpInstruction(const OPCODE opcode, const REG operand1,
                                const FLT32 *operand2,
                                const string *function_name,
                                FpSelector *fp_selector, CONTEXT *ctxt) {
  PIN_REGISTER reg1, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);

  FpOperation operation(opcode, *reg1.flt, *operand2, *function_name);
  FpImplementation *fp_implementation =
      fp_selector->SelectFpImplementation(operation);
  *result.flt = fp_implementation->PerformOperation(operation);
  PIN_SetContextRegval(ctxt, operand1, result.byte);
}

/**
 * Performs any per-function setup needed by the given floating-point selector.
 * This function is called every time a new function is entered in the
 * instrumented application if the KnobFpSelectorName flag is supplied on the
 * command line.
 *
 * @param[in] function_name The name of the function being entered.
 * @param[in,out] fp_selector The floating-point selector.
 */
VOID EnterFunction(const string *function_name, FpSelector *fp_selector) {
  fp_selector->OnFunctionStart(*function_name);
}

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
VOID ExitFunction(const string *function_name, FpSelector *fp_selector) {
  fp_selector->OnFunctionEnd(*function_name);
}

}  // namespace
}  // namespace analysis

namespace callbacks {
namespace {

/**
 * Performs any setup needed by the given floating-point selector.
 * This function is called after Pin initialization is finished but before the
 * instrumented application starts running if the KnobFpSelectorName flag is
 * supplied on the command line.
 *
 * @param[in,out] fp_selector The floating-point selector to setup.
 */
VOID StartCallback(FpSelector *fp_selector) { fp_selector->StartCallback(); }

/**
 * Performs any teardown needed by the given floating-point selector.
 * This function is called immediately before the instrumented application
 * exits if the KnobFpSelectorName flag is supplied on the command line.
 *
 * @param[in] code Exit code of the pintool.
 * @param[in,out] fp_selector The floating-point selector to teardown.
 */
VOID ExitCallback(const INT32 code, FpSelector *fp_selector) {
  fp_selector->ExitCallback(code);
}

/**
 * Schedule calls to analysis routines to replace every floating-point operation
 * in the instrumented application with a user-defined implementation.
 * This function is called every time a new routine is encountered, before the
 * instrumented application is run if the KnobFpSelectorName flag is supplied on
 * the command line.
 *
 * @param[in] rtn Routine to be instrumented.
 * @param[in] fp_selector The floating-point selector to use.
 */
VOID InstrumentationCallback(const RTN rtn, FpSelector *fp_selector) {
  RTN_Open(rtn);
  const string &function_name = RTN_Name(rtn);
  // clang-format off
  RTN_InsertCall(
      rtn, IPOINT_BEFORE,
      reinterpret_cast<AFUNPTR>(analysis::EnterFunction),
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
            reinterpret_cast<AFUNPTR>(analysis::ReplaceRegisterFpInstruction),
            IARG_UINT32, INS_Opcode(ins),
            IARG_UINT32, INS_OperandReg(ins, 0),
            IARG_UINT32, INS_OperandReg(ins, 1),
            IARG_PTR, &function_name,
            IARG_PTR, fp_selector,
            IARG_PARTIAL_CONTEXT, &regs_in, &regs_out,
            IARG_END);
        // clang-format on
      } else {
        // clang-format off
        INS_InsertCall(
            ins, IPOINT_BEFORE,
            reinterpret_cast<AFUNPTR>(analysis::ReplaceMemoryFpInstruction),
            IARG_UINT32, INS_Opcode(ins),
            IARG_UINT32, INS_OperandReg(ins, 0),
            IARG_MEMORYREAD_EA,
            IARG_PTR, &function_name,
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
      reinterpret_cast<AFUNPTR>(analysis::ExitFunction),
      IARG_PTR, &function_name,
      IARG_PTR, fp_selector,
      IARG_END);
  // clang-format on
  RTN_Close(rtn);
}

}  // namespace
}  // namespace callbacks

VOID ReplaceFpOperations(FpSelector *fp_selector) {
  PIN_AddApplicationStartFunction(
      reinterpret_cast<APPLICATION_START_CALLBACK>(callbacks::StartCallback),
      fp_selector);
  PIN_AddFiniFunction(reinterpret_cast<FINI_CALLBACK>(callbacks::ExitCallback),
                      fp_selector);
  RTN_AddInstrumentFunction(reinterpret_cast<RTN_INSTRUMENT_CALLBACK>(
                                callbacks::InstrumentationCallback),
                            fp_selector);
}

}  // namespace ftrace
