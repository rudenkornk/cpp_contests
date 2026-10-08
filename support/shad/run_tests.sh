#!/usr/bin/env bash
set -eu

solution=${1:-./a.out}
test_dir=$(dirname -- "$(realpath -- "${BASH_SOURCE[0]}")")
actual=$(mktemp)
trap 'rm -f -- "$actual"' EXIT
TIMEFORMAT='  Time: %3R s'
failed=0
total=0
shopt -s nullglob

for test in "$test_dir"/test_*; do
  [[ -f "$test" && "${test##*/}" =~ ^test_[0-9]+$ ]] || continue
  total=$((total + 1))
  printf '%s\n' "${test##*/}"
  if time "$solution" <"$test" >"$actual"; then
    if diff -q -B <(tr -s '[:space:]' '\n' <"$actual") \
      <(tr -s '[:space:]' '\n' <"$test.answer") >/dev/null; then
      printf '  OK\n'
    else
      printf '  FAIL: output differs from the expected answer.\n'
      failed=$((failed + 1))
    fi
  else
    printf '  FAIL: program exited with an error.\n'
    failed=$((failed + 1))
  fi
done

if ((total == 0)); then
  printf 'No tests found.\n'
  exit 1
fi

printf '\nPassed: %s/%s\n' "$((total - failed))" "$total"
exit "$((failed != 0))"
