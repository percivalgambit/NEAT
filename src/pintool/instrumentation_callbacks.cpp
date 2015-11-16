#include "pintool/instrumentation_callbacks.h"

#include <pin.H>

#include <iostream>
#include <fstream>
#include <stack>
#include <string>

#include "pintool/common_macros.h"
#include "pintool/instrumentation_args.h"
#include "shared/floating_point_implementation.h"
#include "shared/program_state.h"

namespace ftrace {

VOID StartCallback(const InstrumentationArgs *instrumentation_args) {
  instrumentation_args->floating_point_implementation_->StartCallback();
}

VOID ExitCallback(const INT32 code,
                  const InstrumentationArgs *instrumentation_args) {
  instrumentation_args->floating_point_implementation_->ExitCallback(code);
  delete instrumentation_args;
}

VOID PrintFloatingPointOperation(const OPCODE &operation, const FLT32 &operand1,
                                 const FLT32 &operand2, const FLT32 &result,
                                 ofstream *output_stream) {
  *output_stream << OPCODE_StringShort(operation) << " "
                << FLT32_TO_HEX(operand1) << " " << FLT32_TO_HEX(operand2)
                << "\n";
  *output_stream << "  " << FLT32_TO_HEX(result) << "\n";
}

VOID ReplaceRegisterFloatingPointInstruction(
    const InstrumentationArgs *instrumentation_args,
    const ProgramState *program_state, const OPCODE operation,
    const REG operand1, const REG operand2, CONTEXT *ctxt) {
  PIN_REGISTER reg1, reg2, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);
  PIN_GetContextRegval(ctxt, operand2, reg2.byte);
  FloatingPointImplementation *floating_point_implementation =
      instrumentation_args->floating_point_implementation_;

  *result.flt = floating_point_implementation->FloatingPointOperation(
      *reg1.flt, *reg2.flt, operation, *program_state);
  PIN_SetContextRegval(ctxt, operand1, result.byte);
  if (instrumentation_args->print_floating_point_ops_) {
    PrintFloatingPointOperation(operation, *reg1.flt, *reg2.flt, *result.flt,
                                instrumentation_args->output_stream_);
  }
}

VOID ReplaceMemoryFloatingPointInstruction(
    const InstrumentationArgs *instrumentation_args,
    const ProgramState *program_state, const OPCODE operation,
    const REG operand1, const FLT32 *operand2, CONTEXT *ctxt) {
  PIN_REGISTER reg1, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);
  FloatingPointImplementation *floating_point_implementation =
      instrumentation_args->floating_point_implementation_;

  *result.flt = floating_point_implementation->FloatingPointOperation(
      *reg1.flt, *operand2, operation, *program_state);
  PIN_SetContextRegval(ctxt, operand1, result.byte);
  if (instrumentation_args->print_floating_point_ops_) {
    PrintFloatingPointOperation(operation, *reg1.flt, *operand2, *result.flt,
                                instrumentation_args->output_stream_);
  }
}

VOID CallStackPush(const string *function_name,
                   stack<string> *call_stack) {
  call_stack->push(*function_name);
}

VOID CallStackPop(stack<string> *call_stack) { call_stack->pop(); }

}  // namespace ftrace
