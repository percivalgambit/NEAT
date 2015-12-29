
/** @file
 * This is a PIN tool that traces every floating-point arithmetic operation and
 * displays its arguments and results. This tool can also replace
 * floating-point instructions with arbitrary code to test different
 * floating-point implementations.
 * @note All floating-point values are printed as 8 digit hex numbers padded
 * with 0's.
 */

#include <pin.H>

#include <dlfcn.h>

#include <fstream>
#include <iostream>
#include <string>

#include "pintool/instrument_routine.h"
#include "pintool/instrumentation_args.h"
#include "pintool/instrumentation_callbacks.h"
#include "shared/floating_point_implementation.h"
#include "shared/normal_floating_point_implementation.h"

using ftrace::FloatingPointImplementation;
using ftrace::InstrumentationArgs;

KNOB<BOOL> KnobPrintFloatingPointOperations(
    KNOB_MODE_WRITEONCE, "pintool", "print_floating_point_operations", "0",
    "print the value of every floating point operation in the instrumented "
    "program");

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "ftrace.out",
                            "specify file name for ftrace output");

KNOB<string> KnobFloatingPointImplementationLibrary(
    KNOB_MODE_OVERWRITE, "pintool", "floating_point_implementation_lib", "",
    "specify library from which to load the floating point implementation");

namespace ftrace {

static NormalFloatingPointImplementation normal_floating_point_implementation;

/**
 *  Print out a help message.
 */
INT32 Usage() {
  cerr << "This tool produces a trace of floating point arithmetic "
          "instruction calls and allows those instructions to be replaced "
          "with arbitrary user code to test different floating-point "
          "implementations." << endl;
  cerr << KNOB_BASE::StringKnobSummary() << endl;
  return -1;
}

FloatingPointImplementation *GetFloatingPointImplementationOrDie(
    const string &floating_point_impl_lib_name) {
  void *floating_point_impl_lib =
      dlopen(floating_point_impl_lib_name.c_str(), RTLD_NOW);
  if (floating_point_impl_lib == nullptr) {
    cerr << "No shared library " << floating_point_impl_lib_name
         << " found" << endl;
    exit(1);
  }

#define STR1(str) #str
#define STR2(str) STR1(str)
  void *floating_point_impl =
      dlsym(floating_point_impl_lib, STR2(FLOATING_POINT_IMPL_NAME));
#undef STR1
#undef STR2
  if (floating_point_impl == nullptr) {
    cerr << "No registered floating-point implementation found. "
         << "Make sure REGISTER_FLOATING_POINT_IMPL is called in "
         << floating_point_impl_lib_name << endl;
    exit(1);
  }
  return static_cast<FloatingPointImplementation *>(floating_point_impl);
}

VOID InstrumentProgram(InstrumentationArgs *instrumentation_args) {
  PIN_AddApplicationStartFunction(
      reinterpret_cast<APPLICATION_START_CALLBACK>(StartCallback),
      instrumentation_args);
  PIN_AddFiniFunction(
      reinterpret_cast<FINI_CALLBACK>(ExitCallback), instrumentation_args);
  RTN_AddInstrumentFunction(
      reinterpret_cast<RTN_INSTRUMENT_CALLBACK>(InstrumentRoutine),
      instrumentation_args);
}

}  // namespace ftrace

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
    return ftrace::Usage();
  }

  const BOOL &print_floating_point_ops =
      KnobPrintFloatingPointOperations.Value();
  const string &output_file_name = KnobOutputFile.Value();
  ofstream *output_stream = new ofstream(output_file_name);
  cerr << "===============================================" << endl;
  cerr << "See file " << output_file_name << " for analysis results" << endl;
  cerr << "===============================================" << endl;
  const string &floating_point_impl_lib_name =
      KnobFloatingPointImplementationLibrary.Value();
  FloatingPointImplementation *floating_point_implementation =
      floating_point_impl_lib_name.empty()
          ? &ftrace::normal_floating_point_implementation
          : ftrace::GetFloatingPointImplementationOrDie(
                floating_point_impl_lib_name);

  InstrumentationArgs *instrumentation_args = new InstrumentationArgs(
      print_floating_point_ops, output_stream, floating_point_implementation);
  ftrace::InstrumentProgram(instrumentation_args);
  // Start the program, never returns.
  PIN_StartProgram();
}
