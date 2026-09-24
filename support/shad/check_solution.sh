#!/usr/bin/env bash

set -euo pipefail

if (( $# < 1 || $# > 2 )); then
  printf 'Usage: %s source.cpp [binary]\n' "$0" >&2
  exit 2
fi

source_path=$1
binary_path=${2:-${source_path%.cpp}.out}
config_dir=$(dirname -- "$(realpath -- "${BASH_SOURCE[0]}")")

clang-format --style="file:$config_dir/.clang-format" -i "$source_path"

# clang-tidy does not inherit the wrapped compiler's standard-library header paths in the Nix shell.
include_args=()
in_paths=false
while IFS= read -r line; do
  if [[ $line == '#include <...> search starts here:' ]]; then
    in_paths=true
  elif [[ $line == 'End of search list.' ]]; then
    break
  elif [[ $in_paths == true ]]; then
    include_args+=(-idirafter "${line# }")
  fi
done < <(clang++ -E -v -x c++ /dev/null 2>&1)

clang-tidy --quiet --config-file="$config_dir/.clang-tidy" "$source_path" -- -std=c++20 "${include_args[@]}"
clang++ -std=c++20 -Og -g "$source_path" -o "$binary_path"
