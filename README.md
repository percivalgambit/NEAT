ftrace
======

A PIN tool to trace the value of the arguments and result of each floating point instruction in an IA-32 or x86-64 program.

Currently, the only instructions that this tool instruments are:

    ADDSS
    SUBSS
    MULSS
    DIVSS


Setup
-----

After cloning the repository, run the following commands:

    make
    make PROG=<your_program> run

Alternatively, after making the pintool, to invoke it from the command line run
the following command:

    pin -t obj-intel64/ftrace.so -- <your_program>

For the IA-32 architecture, use `obj-ia32` instead of `obj-intel64`

To see output from the pintool, you must specify an output file with the `-o`
flag after you have specified the `ftrace` tool with the `-t` flag.

Floating-point Instruction Replacement
--------------------------------------

To compile with floating-point instruction replacement code, create one or more
files containing the code you want to compile into the pintool, then add the
correct function and file names to the definitions of the variables in `ftrace.vars`.

See `ftrace.vars` for the different possible insertion points of user-defined
code in the pintool, and see `tests/test_callbacks.cpp` and the files in
`replacement_examples/` for examples of user-defined code that can be compiled
into the pintool.

Testing
-------

To run the tests for this pintool, run the following command:

    make test

Documentation
-------------

Full documentation can be found here: [http://percivalgambit.github.io/ftrace/] (http://percivalgambit.github.io/ftrace/)

Contact
-------

If you have any questions or comments, or find a bug in the code, please contact
[percivalgambit@gmail.com] (mailto:percivalgambit@gmail.com).
