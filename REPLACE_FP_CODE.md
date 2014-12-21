Replacing floating-point instructions
=====================================

When compiling the tool with code to replace floating-point instructions, the type signature of the replacement function should be:

    FLT32 <replacement_fn>(FLT32, FLT32, OPCODE)

The first two function arguments are the two operands to the instruction and the last argument is the opcode of the instruction being replaced.  The file containing the replacement function must also include pin.H as a header file.

See replace_fp_ins_simple.cpp and replace_fp_ins_complex.cpp for examples of replacement functions.
