
/** @file
 * This is a Pin tool that can replace every floating-point arithmetic
 * instruction with user-defined code to test different floating-point
 * implementations. It can also print the arguments and result of every
 * floating-point arithmetic instruction.
 * @note All floating-point values are printed as 8 digit hex numbers padded
 * with 0's.
 */

#include <pin.H>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "client_lib/interfaces/fp_selector.h"
#include "client_lib/registry/internal/fp_selector_registry.h"
#include "pintool/instrument_routine.h"
#include "pintool/instrumentation_callbacks.h"

using ftrace::CloseOutputStream;
using ftrace::ExitCallback;
using ftrace::FpSelector;
using ftrace::InstrumentFpOperations;
using ftrace::PrintFpOperations;
using ftrace::StartCallback;
using ftrace::internal::FpSelectorRegistry;

KNOB<BOOL> KnobPrintFpOps(
    KNOB_MODE_WRITEONCE, "pintool", "print_fp_ops", "0",
    "print the value of every floating point operation in the instrumented "
    "program");

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "ftrace.out",
                            "specify file name for ftrace output");

KNOB<string> KnobFpSelectorName(KNOB_MODE_OVERWRITE, "pintool",
                                "fp_selector_name", "",
                                "specify the name of the FpSelector to use "
                                "when instrumenting an application");

namespace {

/**
 *  Prints out a help message.
 */
INT32 Usage() {
  cerr << "This tool produces a trace of floating point arithmetic "
          "instruction calls and allows those instructions to be replaced "
          "with arbitrary user code to test different floating-point "
          "implementations."
       << endl;
  cerr << KNOB_BASE::StringKnobSummary() << endl;
  return -1;
}

}  // namespace

/**
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet
 * started.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments,
 *                              including pin -t <toolname> -- ...
 */
int main(int argc, char *argv[]) {
  PIN_InitSymbols();
  // Initialize PIN library. Print help message if -h(elp) is specified
  // in the command line or the command line is invalid.
  if (PIN_Init(argc, argv)) {
    return Usage();
  }

  const FpSelectorRegistry *fp_selector_registry =
      FpSelectorRegistry::GetFpSelectorRegistry();
  const string &fp_selector_name = KnobFpSelectorName.Value();
  if (!fp_selector_name.empty()) {
    FpSelector *fp_selector =
        fp_selector_registry->GetFpSelectorOrDie(fp_selector_name);

    PIN_AddApplicationStartFunction(
        reinterpret_cast<APPLICATION_START_CALLBACK>(StartCallback),
        fp_selector);
    PIN_AddFiniFunction(reinterpret_cast<FINI_CALLBACK>(ExitCallback),
                        fp_selector);
    RTN_AddInstrumentFunction(
        reinterpret_cast<RTN_INSTRUMENT_CALLBACK>(InstrumentFpOperations),
        fp_selector);
  }
  const string &output_file_name = KnobOutputFile.Value();
  const BOOL &print_fp_ops = KnobPrintFpOps.Value();
  if (print_fp_ops) {
    ofstream *output_stream = new ofstream(output_file_name.c_str());
    cerr << "===============================================" << endl
         << "See file " << output_file_name << " for analysis results" << endl
         << "===============================================" << endl;

    PIN_AddFiniFunction(reinterpret_cast<FINI_CALLBACK>(CloseOutputStream),
                        output_stream);
    INS_AddInstrumentFunction(
        reinterpret_cast<INS_INSTRUMENT_CALLBACK>(PrintFpOperations),
        output_stream);
  }

  // Start the program, never returns.
  PIN_StartProgram();
}
