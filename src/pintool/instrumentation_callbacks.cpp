#include "pintool/instrumentation_callbacks.h"

#include <pin.H>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "pintool/instrumentation_args.h"
#include "shared/floating_point_implementation.h"
#include "shared/program_state.h"

/*!
 * Convert a FLT32 variable into the string representation of its value in hex.
 * @param[in]   fp  variable to convert to hex
 */
#define FLT32_TO_HEX(fp) \
  StringHex(*reinterpret_cast<const UINT32 *>(&fp), 8, FALSE)

namespace {

VOID PrintFloatingPointOperation(const OPCODE &operation, const FLT32 &operand1,
                                 const FLT32 &operand2, const FLT32 &result,
                                 ofstream *output_stream) {
  *output_stream << OPCODE_StringShort(operation) << " "
                 << FLT32_TO_HEX(operand1) << " " << FLT32_TO_HEX(operand2)
                 << "\n";
  *output_stream << "  " << FLT32_TO_HEX(result) << "\n";
}

}  // namespace

namespace ftrace {

VOID StartCallback(const InstrumentationArgs *instrumentation_args) {
  instrumentation_args->floating_point_implementation->StartCallback();
}

VOID ExitCallback(const INT32 code,
                  const InstrumentationArgs *instrumentation_args) {
  instrumentation_args->floating_point_implementation->ExitCallback(code);
  delete instrumentation_args;
}

VOID ReplaceRegisterFloatingPointInstruction(
    const InstrumentationArgs *instrumentation_args,
    const ProgramState *program_state, const OPCODE operation,
    const REG operand1, const REG operand2, CONTEXT *ctxt) {
  PIN_REGISTER reg1, reg2, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);
  PIN_GetContextRegval(ctxt, operand2, reg2.byte);
  FloatingPointImplementation *floating_point_implementation =
      instrumentation_args->floating_point_implementation;

  *result.flt = floating_point_implementation->FloatingPointOperation(
      *reg1.flt, *reg2.flt, operation, *program_state);
  PIN_SetContextRegval(ctxt, operand1, result.byte);
  if (instrumentation_args->print_floating_point_ops) {
    PrintFloatingPointOperation(operation, *reg1.flt, *reg2.flt, *result.flt,
                                instrumentation_args->output_stream);
  }
}

VOID ReplaceMemoryFloatingPointInstruction(
    const InstrumentationArgs *instrumentation_args,
    const ProgramState *program_state, const OPCODE operation,
    const REG operand1, const FLT32 *operand2, CONTEXT *ctxt) {
  PIN_REGISTER reg1, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);
  FloatingPointImplementation *floating_point_implementation =
      instrumentation_args->floating_point_implementation;

  *result.flt = floating_point_implementation->FloatingPointOperation(
      *reg1.flt, *operand2, operation, *program_state);
  PIN_SetContextRegval(ctxt, operand1, result.byte);
  if (instrumentation_args->print_floating_point_ops) {
    PrintFloatingPointOperation(operation, *reg1.flt, *operand2, *result.flt,
                                instrumentation_args->output_stream);
  }
}

VOID FunctionStackPush(const string *function_name,
                       vector<string> *function_stack) {
  function_stack->push_back(*function_name);
}

VOID FunctionStackPop(vector<string> *function_stack) {
  function_stack->pop_back();
}

}  // namespace ftrace
