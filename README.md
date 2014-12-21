ftrace
======

A PIN tool to trace the value of the arguments and result of each floating point instruction in an IA-32 or x86-64 program.

Setup
-----

After cloning the repository, run the following commands:

    make
    make PROG=<your_program> run

Alternatively, after making the pintool, to invoke it from the commandline run the following command:

    pin -t obj-intel64/ftrace.so -- <your_program>

For the IA-32 architecture, use `obj-ia32` instead of `obj-intel64`

To compile with floating-point instruction replacement code, either specify `REPLACE_FP_FN` and `REPLACE_FP_FILE` in `makefile.rules` or run the following command:

    make REPLACE_FP_FN=<replacement_fn> REPLACE_FP_FILE=<replacement_fn_file>

If the file containing the replacement function is named `<fp_replacement_function>.cpp`, `REPLACE_FP_FILE` can be omitted. Currently only C++ replacement code is supported. See `REPLACE_FP_CODE.md` for more information on the replacement code.

To run the tests for this pintool, run the following command:

    make test

Full documentation can be found here: [http://percivalgambit.github.io/ftrace/] (http://percivalgambit.github.io/ftrace/)
