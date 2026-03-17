#!/usr/bin/env bash

set -e

# This script is an example for use for creduce tool.
# The script cannot be used directly, since it hardcodes specific directory layout.
# But it is useful for reference.
# There are several problems solved (sort of) in this script:
# 1. Allow using cmake for reduction instead of trying to manually extract
#    compile commands from cmake log.
# 2. Allow creduce to run in parallel by not acting on original repo.
#    Instead act inside tmp dir, which creduce creates for us.
#    For that, this script runs rsync from original repo, filtering out redundant git stuff.
# 3. Make the script fast by *not* invoking cmake configuration step from scratch in each tmp dir.
#    Instead just reuse existing build directory, which user should configure beforehand.
#    (This, of course, assumes that time of copying repo+build << time of cmake configure).
#
#    This creates a problem though, since build directory hardcodes build and source paths.
#    For that this script utilizes blunt replacement of build paths with sed.
#    I suspect such replacement may fail in complex cases though.

# 1. Insert original repo path here.
original=$HOME/projects/cpp_contests

rsync -a --exclude='.git' "$original" .
fd -t f . cpp_contests/build -x sed -i  "s|$original|$PWD/cpp_contests|g"

# 2. Tweak copying.
# Here should be happening copying of files, which should be reduced to their RELATIVE location in a source tree.
cp reduced.cppm ./cpp_contests/src/utils/modules/shell.cppm

# 3. Tweak cmake command && interestingness condition.
if cmake --build cpp_contests/build --target utils 2>&1 | grep -q "internal compiler error: Segmentation fault"; then
    echo ICE
    exit 0
else
    exit 1
fi

# 4. After tweaking, set up test environment:
#    1. Prepare build directory, for example `cmake -G Ninja -S . -B build`
#    2. Prepare original version of files to reduce in a separate dir.
#    3. Run `nix develop --command creduce ../reduction/reduce.cppm support/reduce_script.sh`
