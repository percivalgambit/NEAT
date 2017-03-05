#include "pintool/print_function_num_fp_ops.h"

#include <pin.H>

#include <fstream>
#include <map>
#include <string>
#include <utility>

#include "pintool/utils.h"

namespace ftrace {
namespace {

/**
 * Keeps track of the number of floating-point arithmetic operations executed
 * per function in the application.
 */
map<string, UINT64> function_fp_op_count;

/**
 * Lock to protect the map of floating-point arithmetic operations per function
 * so analysis results from multiple calls in multiple threads do not overwrite
 * each other.
 */
PIN_MUTEX function_fp_op_count_lock;

}  // namespace

namespace analysis {
namespace {

/**
 * Increments the count of floating-point artithmetic operations executed in the
 * supplied function name.
 * This function is called for every floating-point arithmetic instruction if
 * the KnobPrintFunctionNumFpOps flag is supplied on the command line.
 *
 * @param[in] function_name The function name of the function executing a
 *     floating-point operation.
 */
VOID IncrementFpFunctionOpCount(const string *function_name) {
  PIN_MutexLock(&function_fp_op_count_lock);
  function_fp_op_count[*function_name]++;
  PIN_MutexUnlock(&function_fp_op_count_lock);
}

}  // namespace
}  // namespace analysis

namespace callbacks {
namespace {

/**
 * Prints the number of floating-point operations executed per function to the
 * supplied output file and closes it.
 * This function is called immediately before the instrumented application
 * exits if the KnobPrintFunctionNumFpOps flag is supplied on the command line.
 *
 * @param[in] code Exit code of the pintool.
 * @param[in,out] output The output file to use.
 */
VOID PrintToFile(const INT32 code, ofstream *output) {
  for (const pair<string, UINT64> &count : function_fp_op_count) {
    *output << count.first << " " << count.second << endl;
  }
  output->close();
  delete output;
  PIN_MutexFini(&function_fp_op_count_lock);
}

/**
 * Schedule calls to analysis routines to count the number of floating-point
 * arithmetic operations that are executed in each function of the instrumented
 * application.
 * This function is called every time a new instruction is encountered, before
 * the instrumented application is run if the KnobPrintFunctionNumFpOps flag is
 * supplied on the command line.
 *
 * @param[in] ins Instruction to be instrumented.
 * @param[in] output The output file to write to.
 */
VOID InstrumentationCallback(const RTN rtn, ofstream *output) {
  RTN_Open(rtn);
  for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)) {
    if (IsFpInstruction(ins)) {
      // clang-format off
      INS_InsertCall(
          ins, IPOINT_BEFORE,
          reinterpret_cast<AFUNPTR>(analysis::IncrementFpFunctionOpCount),
          IARG_PTR, &RTN_Name(rtn),
          IARG_END);
      // clang-format on
    }
  }
  RTN_Close(rtn);
}

}  // namespace
}  // namespace callbacks

VOID PrintFunctionNumFpOps(ofstream *output) {
  PIN_MutexInit(&function_fp_op_count_lock);

  PIN_AddFiniFunction(reinterpret_cast<FINI_CALLBACK>(callbacks::PrintToFile),
                      output);
  RTN_AddInstrumentFunction(reinterpret_cast<RTN_INSTRUMENT_CALLBACK>(
                                callbacks::InstrumentationCallback),
                            output);
}

}  // namespace ftrace
