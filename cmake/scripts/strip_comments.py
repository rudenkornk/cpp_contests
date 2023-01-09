#!/usr/bin/env python3

import argparse as _argparse
import re as _re
import sys as _sys
from pathlib import Path as _Path


def strip_comments(contents: str, comment: str) -> str:
    pattern = f"^{comment}.*"
    regex = _re.compile(pattern, _re.MULTILINE)
    return regex.sub("", contents)


def main() -> None:
    parser = _argparse.ArgumentParser(
        description="Strip comments from input file", formatter_class=_argparse.ArgumentDefaultsHelpFormatter
    )
    parser.add_argument("input", type=_Path, help="path to source file")
    parser.add_argument("-o", "--output", type=_Path, dest="output", help="path to output file")
    parser.add_argument("-c", "--comment", type=str, dest="comment", default="#", help="comment symbols")
    args = parser.parse_args(_sys.argv[1:])

    contents = args.input.read_text(encoding="utf-8")

    stripped = strip_comments(contents, args.comment)
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(stripped, encoding="utf-8")
    else:
        print(stripped)


if __name__ == "__main__":
    main()
