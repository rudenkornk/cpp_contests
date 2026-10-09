#!/usr/bin/env bash
set -eu

solution=${1:-./a.out}
test_dir=$(dirname -- "$(realpath -- "${BASH_SOURCE[0]}")")
actual=$(mktemp)
timing=$(mktemp)
trap 'rm -f -- "$actual" "$timing"' EXIT
LC_NUMERIC=C
TIMEFORMAT='%3R'
failed=0
total=0
total_ms=0
max_ms=0
shopt -s nullglob

for test in "$test_dir"/test_*; do
  [[ -f "$test" && "${test##*/}" =~ ^test_[0-9]+$ ]] || continue
  total=$((total + 1))
  printf '%s\n' "${test##*/}"
  if { time { "$solution" <"$test" >"$actual"; } 2>&3; } 3>&2 2>"$timing"; then
    solution_ok=true
  else
    solution_ok=false
  fi
  read -r elapsed <"$timing"
  elapsed_ms=$((10#${elapsed/./}))
  total_ms=$((total_ms + elapsed_ms))
  if ((elapsed_ms > max_ms)); then
    max_ms=$elapsed_ms
  fi
  printf '  Time: %s s\n' "$elapsed"

  if "$solution_ok"; then
    if [[ -f "$test_dir/compare_answers.sh" ]]; then
      if bash "$test_dir/compare_answers.sh" "$test" "$test.answer" "$actual"; then
        printf '  OK\n'
      else
        failed=$((failed + 1))
      fi
    elif diff -q -B <(tr -s '[:space:]' '\n' <"$actual") \
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
printf 'Total time: %d.%03d s\n' "$((total_ms / 1000))" "$((total_ms % 1000))"
printf 'Max time: %d.%03d s\n' "$((max_ms / 1000))" "$((max_ms % 1000))"
exit "$((failed != 0))"
