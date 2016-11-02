#include "pintool/instrumentation_callbacks.h"

#include <pin.H>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "client_lib/interfaces/fp_implementation.h"
#include "client_lib/interfaces/fp_selector.h"

/*!
 * Convert a FLT32 variable into the string representation of its value as an 8
 * digit hex number, padded with 0's.
 *
 * @param[in] fp Variable to convert.
 */
#define FLT32_TO_HEX(fp) \
  StringHex(*reinterpret_cast<const UINT32 *>(&fp), 8, FALSE)

namespace ftrace {

VOID StartCallback(FpSelector *fp_selector) { fp_selector->StartCallback(); }

VOID ExitCallback(const INT32 code, FpSelector *fp_selector) {
  fp_selector->ExitCallback(code);
}

VOID ReplaceRegisterFpInstruction(const OPCODE operation, const REG operand1,
                                  const REG operand2, FpSelector *fp_selector,
                                  CONTEXT *ctxt) {
  PIN_REGISTER reg1, reg2, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);
  PIN_GetContextRegval(ctxt, operand2, reg2.byte);

  FpImplementation *fp_implementation =
      fp_selector->SelectFpImplementation(*reg1.flt, *reg2.flt, operation);
  *result.flt = fp_implementation->FpOperation(*reg1.flt, *reg2.flt, operation);
  PIN_SetContextRegval(ctxt, operand1, result.byte);
}

VOID ReplaceMemoryFpInstruction(const OPCODE operation, const REG operand1,
                                const FLT32 *operand2, FpSelector *fp_selector,
                                CONTEXT *ctxt) {
  PIN_REGISTER reg1, result;
  PIN_GetContextRegval(ctxt, operand1, reg1.byte);

  FpImplementation *fp_implementation =
      fp_selector->SelectFpImplementation(*reg1.flt, *operand2, operation);
  *result.flt = fp_implementation->FpOperation(*reg1.flt, *operand2, operation);
  PIN_SetContextRegval(ctxt, operand1, result.byte);
}

VOID EnterFunction(const string *function_name, FpSelector *fp_selector) {
  fp_selector->OnFunctionStart(*function_name);
}

VOID ExitFunction(const string *function_name, FpSelector *fp_selector) {
  fp_selector->OnFunctionEnd(*function_name);
}

VOID CloseOutputStream(const INT32 code, ofstream *output) {
  output->close();
  delete output;
}

VOID PrintRegisterFpOperands(const OPCODE operation,
                             const PIN_REGISTER *operand1,
                             const PIN_REGISTER *operand2, ofstream *output) {
  *output << OPCODE_StringShort(operation) << " ";
  // To disambiguate assosiative operations, list the largest operand first.
  if (operation == XED_ICLASS_SUBSS || operation == XED_ICLASS_DIVSS ||
      *operand1->flt > *operand2->flt) {
    *output << FLT32_TO_HEX(operand1->flt) << " "
            << FLT32_TO_HEX(operand2->flt);
  } else {
    *output << FLT32_TO_HEX(operand2->flt) << " "
            << FLT32_TO_HEX(operand1->flt);
  }
  *output << "\n";
}

VOID PrintMemoryFpOperands(const OPCODE operation, const PIN_REGISTER *operand1,
                           const FLT32 *operand2, ofstream *output) {
  *output << OPCODE_StringShort(operation) << " ";
  // To disambiguate assosiative operations, list the largest operand first.
  if (operation == XED_ICLASS_SUBSS || operation == XED_ICLASS_DIVSS ||
      *operand1->flt > *operand2) {
    *output << FLT32_TO_HEX(operand1->flt) << " " << FLT32_TO_HEX(*operand2);
  } else {
    *output << FLT32_TO_HEX(*operand2) << " " << FLT32_TO_HEX(operand1->flt);
  }
  *output << "\n";
}

VOID PrintFpResult(const PIN_REGISTER *result, ofstream *output) {
  *output << "  " << FLT32_TO_HEX(result->flt) << "\n";
}

}  // namespace ftrace
