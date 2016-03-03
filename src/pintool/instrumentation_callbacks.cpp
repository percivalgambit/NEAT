#include "pintool/instrumentation_callbacks.h"

#include <pin.H>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "shared/floating_point_implementation.h"
#include "shared/floating_point_implementation_selector.h"
#include "shared/program_state.h"

namespace {

/*!
 * Convert a FLT32 variable into the string representation of its value in hex.
 * @param[in]   fp  variable to convert to hex
 */
#define FLT32_TO_HEX(fp) \
  StringHex(*reinterpret_cast<const UINT32 *>(&fp), 8, FALSE)

}  // namespace

namespace ftrace {

VOID StartCallback(FloatingPointImplementationSelector
                       *floating_point_implementation_selector) {
  floating_point_implementation_selector->StartCallback();
}

VOID ExitCallback(const INT32 code,
                  FloatingPointImplementationSelector
                      *floating_point_implementation_selector) {
  floating_point_implementation_selector->ExitCallback(code);
}

VOID ReplaceRegisterFloatingPointInstruction(
    const ProgramState *program_state, const OPCODE operation,
    const REG operand1, const REG operand2,
    FloatingPointImplementationSelector *floating_point_implementation_selector,
    CONTEXT *ctxt) {
  PIN_REGISTER reg1, reg2, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);
  PIN_GetContextRegval(ctxt, operand2, reg2.byte);

  FloatingPointImplementation *floating_point_implementation =
      floating_point_implementation_selector->SelectFloatingPointImplementation(
          *program_state);

  *result.flt = floating_point_implementation->FloatingPointOperation(
      *reg1.flt, *reg2.flt, operation);
  PIN_SetContextRegval(ctxt, operand1, result.byte);
}

VOID ReplaceMemoryFloatingPointInstruction(
    const ProgramState *program_state, const OPCODE operation,
    const REG operand1, const FLT32 *operand2,
    FloatingPointImplementationSelector *floating_point_implementation_selector,
    CONTEXT *ctxt) {
  PIN_REGISTER reg1, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);
  FloatingPointImplementation *floating_point_implementation =
      floating_point_implementation_selector->SelectFloatingPointImplementation(
          *program_state);

  *result.flt = floating_point_implementation->FloatingPointOperation(
      *reg1.flt, *operand2, operation);
  PIN_SetContextRegval(ctxt, operand1, result.byte);
}

VOID FunctionStackPush(const string *function_name,
                       vector<string> *function_stack) {
  if (function_stack->empty() || function_stack->back() != *function_name) {
    function_stack->push_back(*function_name);
  }
}

VOID FunctionStackPop(const string *function_name,
                      vector<string> *function_stack) {
  if (!function_stack->empty() && function_stack->back() == *function_name) {
    function_stack->pop_back();
  }
}

VOID CloseOutputStream(const INT32 code, ofstream *output_stream) {
  output_stream->close();
  delete output_stream;
}

VOID PrintRegisterFPOperands(const OPCODE operation,
                             const PIN_REGISTER *operand1,
                             const PIN_REGISTER *operand2,
                             ofstream *output_stream) {
  *output_stream << OPCODE_StringShort(operation) << " "
                 << FLT32_TO_HEX(operand1->flt) << " "
                 << FLT32_TO_HEX(operand2->flt) << "\n";
}

VOID PrintMemoryFPOperands(const OPCODE operation, const PIN_REGISTER *operand1,
                           const FLT32 *operand2, ofstream *output_stream) {
  *output_stream << OPCODE_StringShort(operation) << " "
                 << FLT32_TO_HEX(operand1->flt) << " "
                 << FLT32_TO_HEX(*operand2) << "\n";
}

VOID PrintFPResult(const PIN_REGISTER *result, ofstream *output_stream) {
  *output_stream << "  " << FLT32_TO_HEX(result->flt) << "\n";
}

}  // namespace ftrace
