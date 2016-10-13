ftrace
======

A Pin tool to replace floating-point instructions with user-defined
implementations in an IA-32 or x86-64 program.

Currently, the only instructions that this tool replaces are:

    ADDSS
    SUBSS
    MULSS
    DIVSS


Setup
-----

Before running the tool, make sure that you have the Intel PIN library
downloaded and that the shell variable PIN_ROOT is set to the path where the PIN
library is located.  To build the tool, simply run `make` from the main
directory.  To run the tool, run the following command:

    pin -t obj-intel64/ftrace.so [-fp_selector_name <fp_selector_name>] -- <your_program>

For the IA-32 architecture, use `obj-ia32` instead of `obj-intel64`

You can use the `-print_floating_point_operations` flag to print the operands
and result of every floating-point arithmetic operation in the instrumented
application.

You can specify the name of an output file for the operands and result of each
floating-point operation by using the `-o` flag. The default name of an output
file is `ftrace.out`.

Floating-Point Instruction Replacement
--------------------------------------

Custom floating-point implementation selectors can be written to test the
accuracy of different floating-point implementations. Each `FpSelector` selects
an `FpImplementation` to replace each SSE floating-point arithmetic instruction.
To use an `FpSelector` to instrument an application, the `FpSelector` must first
be registered with one of the classes in `src/client_lib/registry`.  Once an
`FpSelector` has been registered to a name, that name may be used to specify the
`FpSelector` to be used to instrument an application with the
`-fp_selector_name` flag.

See the `tests/` directory for examples of user-defined `FpSelector`s.

Testing
-------

To run the tests for this pintool, run `make test` from the main directory.

Documentation
-------------

Full documentation can be found here: [http://percivalgambit.github.io/ftrace/] (http://percivalgambit.github.io/ftrace/)

Contact
-------

If you have any questions or comments, or find a bug in the code, please contact
[ehudinl@uchicago.edu] (mailto:ehudinl@uchicago.edu).
