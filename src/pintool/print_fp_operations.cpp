#include "pintool/print_fp_operations.h"

#include <pin.H>

#include <fstream>

#include "pintool/utils.h"

/**
 * Convert a FLT32 variable into the string representation of its value as an 8
 * digit hex number, padded with 0's.
 *
 * @param[in] fp Variable to convert.
 */
#define FLT32_TO_HEX(fp) \
  StringHex(*reinterpret_cast<const UINT32 *>(&fp), 8, FALSE)

namespace {

/**
 * Lock to protect the output file supplied in the analysis routines so analysis
 * results from multiples calls in multiple threads do not get intermixed.
 */
PIN_MUTEX output_file_lock;

}  // namespace

namespace ftrace {
namespace analysis {
namespace {

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
                             const PIN_REGISTER *operand2, ofstream *output) {
  // Take control of the output file from this point until the result is written
  // to it
  PIN_MutexLock(&output_file_lock);

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
                           const FLT32 *operand2, ofstream *output) {
  // Take control of the output file from this point until the result is written
  // to it
  PIN_MutexLock(&output_file_lock);

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
VOID PrintFpResult(const PIN_REGISTER *result, ofstream *output) {
  *output << "  " << FLT32_TO_HEX(result->flt) << "\n";

  // Release exclusive hold of the output file
  PIN_MutexUnlock(&output_file_lock);
}

}  // namespace
}  // namespace analysis

namespace callbacks {
namespace {

/**
 * Closes the supplied output file.
 * This function is called immediately before the instrumented application
 * exits if the KnobPrintFpOps flag is supplied on the command line.
 *
 * @param[in] code Exit code of the pintool.
 * @param[in,out] output The output file to close.
 */
VOID CloseOutputStream(const INT32 code, ofstream *output) {
  output->close();
  delete output;
  PIN_MutexFini(&output_file_lock);
}

/**
 * Schedule calls to analysis routines to print the operands and result of every
 * floating-point instruction in the instrumented application.
 * This function is called every time a new instruction is encountered, before
 * the instrumented application is run if the KnobPrintFpOps flag is supplied
 * on the command line.
 *
 * @param[in] ins Instruction to be instrumented.
 * @param[in] output The output file to write to.
 */
VOID InstrumentationCallback(const INS ins, ofstream *output) {
  if (IsFpInstruction(ins)) {
    if (INS_OperandIsReg(ins, 1)) {
      // clang-format off
      INS_InsertCall(
          ins, IPOINT_BEFORE,
          reinterpret_cast<AFUNPTR>(analysis::PrintRegisterFpOperands),
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
          reinterpret_cast<AFUNPTR>(analysis::PrintMemoryFpOperands),
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
        reinterpret_cast<AFUNPTR>(analysis::PrintFpResult),
        IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
        IARG_PTR, output,
        IARG_END);
    // clang-format on
  }
}

}  // namespace
}  // namespace callbacks

VOID PrintFpOperations(ofstream *output) {
  PIN_MutexInit(&output_file_lock);

  PIN_AddFiniFunction(
      reinterpret_cast<FINI_CALLBACK>(callbacks::CloseOutputStream), output);
  INS_AddInstrumentFunction(reinterpret_cast<INS_INSTRUMENT_CALLBACK>(
                                callbacks::InstrumentationCallback),
                            output);
}

}  // namespace ftrace
