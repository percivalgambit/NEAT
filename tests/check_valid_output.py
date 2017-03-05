#!/usr/bin/env python3
"""
Checks that the supplied file on the command line to ensure that it is valid
output of the -print_fp_ops flag.
"""

import sys
import re

OP_REGEX = re.compile(r"(ADDSS|SUBSS|MULSS|DIVSS) [0-9a-f]{8} [0-9a-f]{8}")
RESULT_REGEX = re.compile(r"  [0-9a-f]")


def main():
    if len(sys.argv) != 2:
        sys.stderr.write("{} expected 1 argument, {} given\n".format(sys.argv[
            0], len(sys.argv) - 1))
        sys.exit(1)

    with open(sys.argv[1], "r") as f:
        lines = f.read().split("\n")
    if lines[-1] == "":
        lines = lines[:-1]
    if len(lines) % 2 != 0:
        sys.stderr.write(
            "Expected an even number of lines in {}\n".format(sys.argv[1]))
        sys.exit(1)

    for i in range(0, len(lines), 2):
        if OP_REGEX.match(lines[i]) is None:
            sys.stderr.write(
                "'{}' did not match the operation regex\n".format(lines[i]))
            sys.exit(1)
        if RESULT_REGEX.match(lines[i + 1]) is None:
            sys.stderr.write(
                "'{}' did not match the result regex\n".format(lines[i + 1]))
            sys.exit(1)


if __name__ == "__main__":
    main()
