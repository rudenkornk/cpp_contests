#!/usr/bin/env python3

import argparse
import os
import re
import sys


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Find appropriate conan profile matching query and generate conan options for it.",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("query", type=str, help="regex query for desired profile")
    args = parser.parse_args(sys.argv[1:])
    re_compiled = re.compile(args.query)

    profiles = os.popen("conan profile list").readlines()
    profiles = [profile.strip() for profile in profiles]
    filtered_profiles = [profile for profile in profiles if re_compiled.search(profile)]
    if len(filtered_profiles) == 0:
        eprint("WARNING: No matching conan profiles found for query '{}'.".format(args.query))
        eprint("Will try to use default profile.")
        print("")
        exit(0)
    if len(filtered_profiles) > 1:
        eprint("WARNING: Found more than one matching profile for query '{}':".format(args.query))
        eprint("\n".join(filtered_profiles))
        eprint("Will be picked the first one.")
    print("--profile:host {0} --profile:build {0}".format(filtered_profiles[0]))
