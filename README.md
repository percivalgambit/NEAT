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
For the IA-32 architecture, use "obj-ia32" instead of "obj-intel64"

To run the tests for this pintool, run the following command:

    make test
