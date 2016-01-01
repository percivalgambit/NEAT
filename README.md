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

After cloning the repository, run the following commands:

    make FLOATING_POINT_IMPL_LIB=<floating_point_impl_lib_base_name> FLOATING_POINT_IMPL_LIS_SRCS=<floating_point_impl_lib_srcs>
    pin -injection child -ifeellucky -t obj-intel64/ftrace.so -floating_point_implementation_lib <floating_point_impl_lib_name>  -- <your_program>

For the IA-32 architecture, use `obj-ia32` instead of `obj-intel64`

You can specify the name of an output file by using the `-o` flag. The default
name of an output file is `ftrace.out`.

You can use the `-print_floating_point_operations` to print the operands and
result of every floating-point arithmetic operation in the instrumented
application.

Floating-Point Instruction Replacement
--------------------------------------

To compile a library with a user-defined implementation of floating-point
arithmetic instructions, create one or more files containing the code you want
to compile into the library.  These files must contain exactly one subclass of
the `FloatingPointImplementation` class that is registered with
`REGISTER_FLOATING_POINT_IMPL`.  Finally, run the following command:

    make FLOATING_POINT_IMPL_LIB=<floating_point_impl_lib_base_name> FLOATING_POINT_IMPL_LIS_SRCS=<floating_point_impl_lib_srcs>

See the `tests/` directory for examples of user-defined floating-point
implementations.

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
[ehudinl@uchicago.edu] (mailto:ehudinl@uchicago.edu).
