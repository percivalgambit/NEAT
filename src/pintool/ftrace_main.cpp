
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

#include <iostream>
#include <fstream>
#include <string>

#include "client/floating_point_implementation.h"
#include "pintool/ftrace.h"

using ftrace::FloatingPointImplementation;

/* ===================================================================== */
// Command line switches
/* ===================================================================== */

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "",
                            "specify file name for ftrace output");

KNOB<string> KnobFloatingPointImplementationLibrary(
    KNOB_MODE_OVERWRITE, "pintool", "floating_point_implementation_lib", "",
    "specify library from which to load the floating point implementation");

KNOB<BOOL> KnobPrintFloatingPointOperations(
    KNOB_MODE_WRITEONCE, "pintool", "print_floating_point_operations", "0",
    "print the value of every floating point operation in the instrumented "
    "program");

static void *floating_point_impl_lib;

namespace ftrace {

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
  floating_point_impl_lib =
      dlopen(floating_point_impl_lib_name.c_str(), RTLD_NOW);
  if (floating_point_impl_lib == nullptr) {
    ERROR("No shared library " << floating_point_impl_lib_name << " found");
  }

  void *floating_point_impl =
      dlsym(floating_point_impl_lib, FLOATING_POINT_IMPL_NAME);
  if (floating_point_impl == nullptr) {
    ERROR("No registered floating-point implementation found. "
          << "Make sure REGISTER_FLOATING_POINT_IMPL is called in "
          << floating_point_impl_lib_name);
  }
  return reinterpret_cast<FloatingPointImplementation *>(floating_point_impl);
}

VOID StartCallback(VOID *floating_point_impl) {
  reinterpret_cast<FloatingPointImplementation *>(
      floating_point_impl)->StartCallback();
}

VOID ExitCallback(const INT32 code, VOID *floating_point_impl) {
  reinterpret_cast<FloatingPointImplementation *>(
      floating_point_impl)->ExitCallback(code);
  if (floating_point_impl_lib != nullptr && dlclose(floating_point_impl_lib)) {
    ERROR("Error closing " << KnobFloatingPointImplementationLibrary.Value());
  }
}

VOID InstrumentProgram(
    FloatingPointImplementation *floating_point_impl) {
  if (floating_point_impl != nullptr) {
    PIN_AddApplicationStartFunction(
        StartCallback, reinterpret_cast<VOID *>(floating_point_impl));
    PIN_AddFiniFunction(
        ExitCallback, reinterpret_cast<VOID *>(floating_point_impl));
  }
  BOOL print_floating_point_ops = KnobPrintFloatingPointOperations.Value();
  if (floating_point_impl != nullptr || print_floating_point_ops) {
    RoutineInstrumentationArgs *instrumentation_args =
        new RoutineInstrumentationArgs(floating_point_impl,
                                       print_floating_point_ops);
    RTN_AddInstrumentFunction(Routine, instrumentation_args);
  }
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

  if (!KnobOutputFile.Value().empty()) {
    // Redirect cout to the output file specified.
    const ofstream out(KnobOutputFile.Value());
    cout.rdbuf(out.rdbuf());

    cerr <<  "===============================================" << endl;
    cerr << "See file " << KnobOutputFile.Value() << " for analysis results"
         << endl;
    cerr <<  "===============================================" << endl;
  }

  const string &floating_point_impl_lib_name =
      KnobFloatingPointImplementationLibrary.Value();
  FloatingPointImplementation *floating_point_impl = nullptr;
  if (!floating_point_impl_lib_name.empty()) {
    floating_point_impl = ftrace::GetFloatingPointImplementationOrDie(
        floating_point_impl_lib_name);
  }

  ftrace::InstrumentProgram(floating_point_impl);
  // Start the program, never returns.
  PIN_StartProgram();

  return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
