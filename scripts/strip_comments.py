#!/usr/bin/env python3

import argparse
from pathlib import Path
import re
import sys
import os


def strip_comments(contents: str, comment: str):
    pattern = r"^{}.*".format(comment)
    regex = re.compile(pattern, re.MULTILINE)
    return regex.sub("", contents)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Strip comments from input file",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("input", type=Path, help="path to source file")
    parser.add_argument("-o", "--output", type=Path,
                        dest="output", help="path to output file")
    parser.add_argument("-c", "--comment", type=str, dest="comment",
                        default="#", help="comment symbols")
    args = parser.parse_args(sys.argv[1:])

    with open(args.input, "r") as file:
        contents = file.read()

    stripped = strip_comments(contents, args.comment)
    if args.output:
        with open(args.output, "w") as file:
            file.write(stripped)
    else:
        print(stripped)
