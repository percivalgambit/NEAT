
/** @file
 * This is a Pin tool that can replace every floating-point arithmetic
 * instruction with user-defined code to test different floating-point
 * implementations. It can also print the arguments and result of every
 * floating-point arithmetic instruction.
 * @note All floating-point values are printed as 8 digit hex numbers padded
 * with 0's.
 */

#include <pin.H>

#include <dlfcn.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "pintool/instrument_routine.h"
#include "pintool/instrumentation_callbacks.h"
#include "shared/floating_point_implementation_selector.h"
#include "shared/internal/fp_selector_registry.h"

using ftrace::CloseOutputStream;
using ftrace::ExitCallback;
using ftrace::FloatingPointImplementationSelector;
using ftrace::FpSelectorRegistry;
using ftrace::InstrumentFPOperations;
using ftrace::PrintFPOperations;
using ftrace::StartCallback;

KNOB<BOOL> KnobPrintFloatingPointOperations(
    KNOB_MODE_WRITEONCE, "pintool", "print_floating_point_operations", "0",
    "print the value of every floating point operation in the instrumented "
    "program");

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "ftrace.out",
                            "specify file name for ftrace output");

KNOB<string> KnobFpSelectorName(KNOB_MODE_OVERWRITE, "pintool",
                                "fp_selector_name", "",
                                "specify the name of the "
                                "FloatingPointImplementationSelector to use "
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

#ifndef FP_SELECTOR_REGISTRY_LIB_PATH
#error FP_SELECTOR_REGISTRY_LIB_PATH must be defined.
#endif

#define TOKEN_TO_STRING(str) #str
#define MACRO_TO_STRING(str) TOKEN_TO_STRING(str)
/**
 * Loads the floating point implementation from the library with the given
 * name or crashes the program if an error occurs.
 * @param[in]   floating_point_impl_lib_name      name of the library to open
 * @returns the loaded floating-point implementation
 */
FpSelectorRegistry *GetFpSelectorRegistryOrDie() {
  void *fp_selector_registry_lib =
      dlopen(MACRO_TO_STRING(FP_SELECTOR_REGISTRY_LIB_PATH), RTLD_LAZY);
  if (fp_selector_registry_lib == nullptr) {
    cerr << "No shared library "
         << MACRO_TO_STRING(FP_SELECTOR_REGISTRY_LIB_PATH) << " found" << endl;
    cerr << "Please run 'make clean' then 'make' to fix" << endl;
    exit(1);
  }

  void *fp_selector_registry = dlsym(
      fp_selector_registry_lib, MACRO_TO_STRING(FP_SELECTOR_REGISTRY_NAME));
  if (fp_selector_registry_lib == nullptr) {
    cerr << "No registry found in "
         << MACRO_TO_STRING(FP_SELECTOR_REGISTRY_LIB_PATH) << endl;
    cerr << "Please run 'make clean' then 'make' to fix" << endl;
    exit(1);
  }
  return static_cast<FpSelectorRegistry *>(fp_selector_registry);
}
#undef TOKEN_TO_STRING
#undef MACRO_TO_STRING

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

  const FpSelectorRegistry *fp_selector_registry = GetFpSelectorRegistryOrDie();
  const string &fp_selector_name = KnobFpSelectorName.Value();
  if (!fp_selector_name.empty()) {
    FloatingPointImplementationSelector
        *floating_point_implementation_selector =
            fp_selector_registry->GetFpSelectorOrDie(fp_selector_name);

    PIN_AddApplicationStartFunction(
        reinterpret_cast<APPLICATION_START_CALLBACK>(StartCallback),
        floating_point_implementation_selector);
    PIN_AddFiniFunction(reinterpret_cast<FINI_CALLBACK>(ExitCallback),
                        floating_point_implementation_selector);
    RTN_AddInstrumentFunction(
        reinterpret_cast<RTN_INSTRUMENT_CALLBACK>(InstrumentFPOperations),
        floating_point_implementation_selector);
  }
  const string &output_file_name = KnobOutputFile.Value();
  const BOOL &print_floating_point_ops =
      KnobPrintFloatingPointOperations.Value();
  if (print_floating_point_ops) {
    ofstream *output_stream = new ofstream(output_file_name.c_str());
    cerr << "===============================================" << endl
         << "See file " << output_file_name << " for analysis results" << endl
         << "===============================================" << endl;

    PIN_AddFiniFunction(reinterpret_cast<FINI_CALLBACK>(CloseOutputStream),
                        output_stream);
    INS_AddInstrumentFunction(
        reinterpret_cast<INS_INSTRUMENT_CALLBACK>(PrintFPOperations),
        output_stream);
  }

  // Start the program, never returns.
  PIN_StartProgram();
}
