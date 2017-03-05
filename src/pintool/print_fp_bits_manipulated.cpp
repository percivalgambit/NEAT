#include "pintool/print_fp_bits_manipulated.h"

#include <pin.H>

#include <strings.h>

#include <fstream>
#include <unordered_map>
#include <utility>

#include "pintool/utils.h"

namespace ftrace {
namespace {

/**
 * Keeps track of the number of bits manipulated in every floating-point
 * arithmetic operation per thread in the application.
 */
unordered_map<THREADID, UINT64> fp_bits_manipulated;

/**
 * Counts the number of bits used in the matissa of the supplied floating-point
 * number and adds the number to a running total.
 *
 * @param[in] flt The floating-point number.
 * @note The number of bits used in the matissa is recorded as 23 minus the
 *     least significant set bit in the mantissa.
 */
VOID CountFpMantissaBits(FLT32 flt, THREADID threadid) {
  INT32 bits = *reinterpret_cast<INT32 *>(&flt);
  // Zero out all bits before the mantissa of the floating-point number.
  bits &= 0x007fffff;
  // We use the ffs function to find the least significant bit set in the
  // mantissa, and we need to subtract this number from 24 because it is
  // 1-indexed.
  if (bits != 0) {
    fp_bits_manipulated[threadid] += 24 - ffs(bits);
  }
}

}  // namespace

namespace analysis {
namespace {

/**
 * Counts the number of bits used in both operands of a floating-point
 * arithmetic operation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on two registers if the KnobPrintFpBitsManipulated flag is supplied
 * on the command line.
 *
 * @param[in] operand1 First operand of the instruction.
 * @param[in] operand2 Second operand of the instruction.
 * @note The number of bits used is recorded as 23 minus the least significant
 *     set bit in the mantissa.
 */
VOID CountRegisterFpOperandBits(const PIN_REGISTER *operand1,
                                const PIN_REGISTER *operand2,
                                const THREADID threadid) {
  CountFpMantissaBits(*operand1->flt, threadid);
  CountFpMantissaBits(*operand2->flt, threadid);
}

/**
 * Counts the number of bits used in both operands of a floating-point
 * arithmetic operation.
 * This function is called for every floating-point arithmetic instruction that
 * operates on a register and a memory location if the
 * KnobPrintFpBitsManipulated flag is supplied on the command line.
 *
 * @param[in] operand1 First operand of the instruction.
 * @param[in] operand2 Second operand of the instruction.
 * @note The number of bits used is recorded as 23 minus the least significant
 *     set bit in the mantissa.
 */
VOID CountMemoryFpOperandBits(const PIN_REGISTER *operand1,
                              const FLT32 *operand2, const THREADID threadid) {
  CountFpMantissaBits(*operand1->flt, threadid);
  CountFpMantissaBits(*operand2, threadid);
}

/**
 * Counts the number of bits used in the result of a floating-point arithmetic
 * operation.
 * This function is called for every floating-point arithmetic instruction if
 * the KnobPrintFpBitsManipulated flag is supplied on the command line.
 *
 * @param[in] result Result of the instruction.
 * @note The number of bits used is recorded as 23 minus the least significant
 *     set bit in the mantissa.
 */
VOID CountFpResultBits(const PIN_REGISTER *result, const THREADID threadid) {
  CountFpMantissaBits(*result->flt, threadid);
}

}  // namespace
}  // namespace analysis

namespace callbacks {
namespace {

/**
 * Prints the number of bits used in floating-point operations to the supplied
 * output file and closes it.
 * This function is called immediately before the instrumented application
 * exits if the KnobPrintFpBitsManipulated flag is supplied on the command
 * line.
 *
 * @param[in] code Exit code of the pintool.
 * @param[in,out] output The output file to use.
 */
VOID PrintToFile(const INT32 code, ofstream *output) {
  UINT64 total_bits_manipulated = 0;
  for (const pair<THREADID, UINT64> &thread_count : fp_bits_manipulated) {
    total_bits_manipulated += thread_count.second;
  }

  *output << total_bits_manipulated << endl;
  output->close();
  delete output;
}

/**
 * Schedule calls to analysis routines to count the number of bits used in every
 * floating-point operation in the instrmented application.
 * This function is called every time a new instruction is encountered, before
 * the instrumented application is run if the KnobPrintFpBitsManipulated flag
 * is supplied on the command line.
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
          reinterpret_cast<AFUNPTR>(analysis::CountRegisterFpOperandBits),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 1),
          IARG_THREAD_ID,
          IARG_CALL_ORDER, CALL_ORDER_FIRST,
          IARG_END);
      // clang-format on
    } else {
      // clang-format off
      INS_InsertCall(
          ins, IPOINT_BEFORE,
          reinterpret_cast<AFUNPTR>(analysis::CountMemoryFpOperandBits),
          IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
          IARG_MEMORYREAD_EA,
          IARG_THREAD_ID,
          IARG_CALL_ORDER, CALL_ORDER_FIRST,
          IARG_END);
      // clang-format on
    }

    // clang-format off
    INS_InsertCall(
        ins, IPOINT_AFTER,
        reinterpret_cast<AFUNPTR>(analysis::CountFpResultBits),
        IARG_REG_CONST_REFERENCE, INS_OperandReg(ins, 0),
        IARG_THREAD_ID,
        IARG_END);
    // clang-format on
  }
}

}  // namespace
}  // namespace callbacks

VOID PrintFpBitsManipulated(ofstream *output) {
  PIN_AddFiniFunction(reinterpret_cast<FINI_CALLBACK>(callbacks::PrintToFile),
                      output);
  INS_AddInstrumentFunction(reinterpret_cast<INS_INSTRUMENT_CALLBACK>(
                                callbacks::InstrumentationCallback),
                            output);
}

}  // namespace ftrace
