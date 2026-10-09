#!/usr/bin/env bash

set -euo pipefail

debug=false
while getopts ':d' option; do
  case $option in
    d) debug=true ;;
    \?)
      printf 'Usage: %s [-d] [source.cpp [binary]]\n' "$0" >&2
      exit 2
      ;;
  esac
done
shift "$((OPTIND - 1))"

if (( $# > 2 )); then
  printf 'Usage: %s [-d] [source.cpp [binary]]\n' "$0" >&2
  exit 2
fi

source_path=${1:-a.cpp}
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
compile_flags=(-O2)
if [[ $debug == true ]]; then
  compile_flags=(-O0 -g)
fi

clang++ -std=c++20 "${compile_flags[@]}" "$source_path" -o "$binary_path"
