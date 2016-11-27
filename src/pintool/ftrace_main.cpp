/**
 * This is a Pin tool that can replace every floating-point arithmetic
 * instruction with user-defined functions to test different floating-point
 * implementations. It can also print the arguments and result of every
 * floating-point arithmetic instruction.
 *
 * @note All floating-point values are printed as 8 digit hex numbers padded
 * with 0's.
 */

#include <pin.H>

#include <fstream>
#include <iostream>
#include <string>

#include "client_lib/interfaces/fp_selector.h"
#include "client_lib/registry/internal/fp_selector_registry.h"
#include "pintool/print_fp_bits_manipulated.h"
#include "pintool/print_fp_operations.h"
#include "pintool/print_function_num_fp_ops.h"
#include "pintool/replace_fp_operations.h"

using ftrace::FpSelector;
using ftrace::PrintFpBitsManipulated;
using ftrace::PrintFpOperations;
using ftrace::PrintFunctionNumFpOps;
using ftrace::ReplaceFpOperations;
using ftrace::internal::FpSelectorRegistry;

KNOB<string> KnobFpSelectorName(KNOB_MODE_OVERWRITE, "pintool",
                                "fp_selector_name", "",
                                "specify the name of the FpSelector to use "
                                "when instrumenting an application");

KNOB<string> KnobPrintFpOps(
    KNOB_MODE_OVERWRITE, "pintool", "print_fp_ops", "",
    "print the value of every floating point operation in the instrumented "
    "program to the specified log file");

KNOB<string> KnobPrintFpBitsManipulated(KNOB_MODE_OVERWRITE, "pintool",
                                        "print_fp_bits_manipulated", "",
                                        "print the total number of bits "
                                        "manipulated in every floating point "
                                        "operation in the instrumented program "
                                        "to the specified log file");

KNOB<string> KnobPrintFunctionNumFpOps(
    KNOB_MODE_OVERWRITE, "pintool", "print_function_num_fp_ops", "",
    "print the number of floating point "
    "operations executed per function of the "
    "instrumented application to the "
    "specified log file");

/**
 *  Prints out a help message.
 *
 *  @return An error code for the application.
 */
static INT32 Usage() {
  cerr << "This is a Pin tool that can replace every floating-point arithmetic "
          "instruction with user-defined functions to test different "
          "floating-point implementations. It can also print the arguments and "
          "result of every floating-point arithmetic instruction."
       << endl;
  cerr << KNOB_BASE::StringKnobSummary() << endl;
  return -1;
}

/**
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet
 * started.
 *
 * @param[in] argc Total number of elements in the argv array
 * @param[in] argv Array of command line arguments,
 *     including pin -t <toolname> -- ...
 */
int main(int argc, char *argv[]) {
  // Initialize program symbols for use by PIN
  PIN_InitSymbols();
  // Initialize PIN library. Print help message if -h(elp) is specified
  // in the command line or the command line is invalid.
  if (PIN_Init(argc, argv)) {
    return Usage();
  }

  // If the KnobFpSelectorName flag is specified on the command line, attempt to
  // look up the FpSelector from the registry and use it to instrument the
  // application program with a user-defined FP implementation if it is found.
  const FpSelectorRegistry *fp_selector_registry =
      FpSelectorRegistry::GetFpSelectorRegistry();
  const string &fp_selector_name = KnobFpSelectorName.Value();
  if (!fp_selector_name.empty()) {
    FpSelector *fp_selector =
        fp_selector_registry->GetFpSelectorOrDie(fp_selector_name);
    ReplaceFpOperations(fp_selector);
  }

  // If the KnobPrintFpOps flag is specified on the command line, instrument the
  // application program to print the arguments and result of every FP operation
  // formatted as 8 digit hex numbers padded with 0's to a file.
  const string &print_fp_ops_file_name = KnobPrintFpOps.Value();
  if (!print_fp_ops_file_name.empty()) {
    ofstream *print_fp_ops_output =
        new ofstream(print_fp_ops_file_name.c_str());
    PrintFpOperations(print_fp_ops_output);
  }

  // If the KnobPrintFpBitsManipulated flag is specified on the command line,
  // instrument the application program to print the total number of bits
  // manipulated in every floating-point operation.
  const string &print_fp_bits_file_name = KnobPrintFpBitsManipulated.Value();
  if (!print_fp_bits_file_name.empty()) {
    ofstream *print_fp_bits_output =
        new ofstream(print_fp_bits_file_name.c_str());
    PrintFpBitsManipulated(print_fp_bits_output);
  }

  // If the KnobPrintFunctionNumFpOps flag is specified on the command line,
  // instrument the application program to print the number of floating-point
  // operations executed per function in the application.
  const string &print_function_num_fp_ops_file_name =
      KnobPrintFunctionNumFpOps.Value();
  if (!print_function_num_fp_ops_file_name.empty()) {
    ofstream *print_function_num_fp_ops_output =
        new ofstream(print_function_num_fp_ops_file_name.c_str());
    PrintFunctionNumFpOps(print_function_num_fp_ops_output);
  }

  // Start the program, never returns.
  PIN_StartProgram();
}
