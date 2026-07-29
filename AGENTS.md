# AGENTS.md: AI Collaboration Guide

This document provides essential context for AI models interacting with this repository.
Following these guidelines ensures consistent architecture, coding style, and development practices.

---

## 1. Project Overview & Purpose

**Primary Goal:**
This repository contains solutions for different C++ educational problems.
The project serves as a collection of algorithmic challenges and data structure implementations.
It functions as both a learning resource and a demonstration of modern C++ development practices.

**Business Domain:**
The project operates in the domain of educational programming and algorithmic problem-solving.
It focuses on implementing classic computer science problems such as cache algorithms, geometry computations, and data structure manipulations.
The codebase demonstrates modern C++ features and best practices in systems programming.

---

## 2. Core Technologies & Stack

**Languages:**
C++23 is the primary language used throughout the project.
C++20 modules are extensively utilized for code organization.

**Compilers:**
GCC 15 is explicitly used as evidenced by the Nix flake configuration.
Clang is also supported as a primary compiler.
MSVC support is considered but not fully implemented for all features.

**Build System:**
CMake 4.0 is the primary build system.
Ninja is used as the CMake backend generator.
The project uses CMake presets for configuration management.

**Dependency Management:**
Nix is the primary dependency management solution via `flake.nix`.
The Nix flake provides reproducible development environments.
Development shells are configured via `nix develop` for all required dependencies.
Boost is managed through the Nix ecosystem.
No vcpkg or Conan configuration files are present in the project.
Dependencies are declaratively specified in the Nix flake inputs.

**Nix Ecosystem Usage:**
The project heavily relies on Nix for reproducible builds and development environments.
`flake.nix` defines build inputs, native build inputs, and development tools.
The flake follows nixpkgs version 26.05 for package versions.
Development shell includes compilers (GCC, Clang), build tools (CMake, Ninja), testing tools (Valgrind, gcovr), and linters.
The project can be built as a Nix package via `nix build`.
All CI/CD workflows utilize Nix for dependency provisioning.

**Testing Frameworks:**
Boost.Test (Boost Unit Test Framework) is used for unit testing.
Tests are integrated with CMake's CTest framework.
Custom test harness utilities are provided in the `utils` module.

**Key Libraries:**
Boost is the primary external dependency, specifically `program_options` and `unit_test_framework`.
The standard library is heavily utilized with modern C++23 features.
Shell command execution in utility code is implemented using POSIX `fork`/`exec` plus a single-threaded
`poll()` I/O multiplexer that drains stdout/stderr while feeding stdin (no Boost.Process dependency for now).

**Platforms:**
Linux is the primary supported platform.
Ubuntu 24.04 is used in CI/CD environments.
x86_64-linux architecture is explicitly targeted in the Nix configuration.

---

## 3. Architectural Patterns

**Overall Architecture:**
The project follows a modular library architecture.
Each problem solution is organized as an independent module with its own CMake configuration.
Modules export C++20/23 module interfaces (`.cppm` files) for modern code organization.
A shared `utils` module provides common functionality across all problem solutions.

**Directory Structure Philosophy:**
`/src` contains all problem solution subdirectories and the shared utilities module.
Each problem subdirectory contains its own module definitions, CLI executables, tests, and CMakeLists.txt.
`/cmake` contains build system helper modules, presets, and installation configuration.
`/cmake/modules` contains reusable CMake functions for coding standards, testing, and installation.
`/cmake/presets` defines compiler-specific configuration presets (GCC and LLVM).
`/.github/workflows` contains CI/CD pipeline definitions.
`/build` is the generated build directory (excluded from version control).
No separate `/include` directory exists; headers are colocated with implementation.

**Module Organization:**
The project uses C++20 modules with the `.cppm` extension for module interface units.
Each problem solution is namespaced under `cpp_contests`.
Modules follow a naming pattern like `geometry.matrix`, `lru_cache.solution`.
The `utils` module provides header-based utilities with the `cpp_contests` namespace.
Strong encapsulation is achieved through module exports.

---

## 4. Coding Conventions & Style Guide

**Formatting:**
`.clang-format` is present with a column limit of 120 characters.
`cmake-format` is used for CMake file formatting.
`prettier` is used for formatting other file types (JSON, YAML, Markdown).
Formatting is enforced via a CMake target `format`.
CI/CD checks ensure code is formatted before merging.

**Naming Conventions:**
Functions use `snake_case` (e.g., `size_to_string`, `get_indices`, `lru_hits`).
Classes use `PascalCase` (e.g., `LRUCache`, `TransposeIterator`, `SaveRestore`).
Template parameters use `PascalCase` (e.g., `Iterator`, `Key`, `Value`).
Namespaces use `snake_case` (primary namespace is `cpp_contests`).
Private member variables use `snake_case_` with trailing underscore (e.g., `load_`, `max_length_`).
Constants use `SCREAMING_SNAKE_CASE` (e.g., `EPSMIN`, `EPSMAX`).
Boolean predicates follow conventional naming (e.g., `is_exact`).

**API Design:**
Public APIs are exported through C++20 modules.
RAII is extensively used for resource management.
Template metaprogramming is employed with concepts and type traits.
Strong type safety is enforced through static assertions.
Member functions are marked `const` and `noexcept` where appropriate.
Explicit constructors prevent implicit conversions.
Copy and move semantics are carefully controlled (often deleted for complex types).

---

## 5. Comment Style Guidelines

These rules apply to comments in all source files (C++, CMake, Nix, shell, etc.) as well as to prose text in
Markdown files.

### Rule 1: Comments are prose

Comments and Markdown prose should be treated as continuous text formatted as paragraphs, with proper punctuation
and capitalization. Even a single-sentence comment must start with a capital letter and end with punctuation
(`.`, `!`, or `?`).

**Exceptions:**

- If the comment starts with a backtick-quoted code reference, the capitalization follows the identifier's own casing.
- If the last word of a comment is a URL, do not append a trailing dot — URL pickers may misparse it.
  Start a new line for the next sentence instead.

**Good:**

```cpp
// By default the cache retains every inserted key. Passing a capacity evicts the least-recently-used entry first.
auto cache = LRUCache<Key, Value>{capacity};

// `is_exact` reports whether the intersection was computed without any rounding.
return result.is_exact();
```

**Bad:**

```cpp
// evict the oldest entry once the cache is full   ← missing capital letter and terminating punctuation
evict_lru();
```

### Rule 2: Line length ≤ 120 characters

No comment line may exceed 120 characters. When a sentence does not fit, split it at a meaningful boundary —
after a comma, or before a conjunction such as "and", "or", "which". A sentence that fits within 120 chars
may still be split across lines.

**Good:**

```cpp
// `two_queue_hits` sizes the virtual value so the per-key overhead stays within the cache budget,
// which keeps the simulated hit rate comparable to the reference implementation.
```

**Bad:**

```cpp
// `two_queue_hits` sizes the virtual value so the per-key overhead stays within the cache budget, which keeps the
// simulated hit rate comparable to the reference implementation.   ← split at a poor boundary
```

### Rule 3: One sentence per line (generally)

Different sentences should generally each start on their own line. Two short sentences may share a line if together
they fit within 120 characters. A sentence must never be split across a line boundary with another sentence mixed in.

**Good:**

```cpp
// The permutation is validated on a non-mutating copy.
// Building it with ranges avoids an extra allocation.
// Setup is cumbersome. In the end the bottleneck was very slow validation.
```

**Bad:**

```cpp
// The permutation is validated on a non-mutating copy. Building it with
// ranges avoids an extra allocation.
```

---

## 6. Common C++ Patterns & Idioms

**Memory Management:**
RAII is the primary memory management pattern throughout the codebase.
Smart pointers are used where dynamic allocation is necessary.
Move semantics are extensively utilized (e.g., `std::move`, perfect forwarding).
Static assertions ensure nothrow guarantees for move operations.
Explicit lifetime management through RAII wrappers like `SaveRestore`.

**Error Handling:**
Exceptions are used for error propagation.
The `ExceptionSaver` utility captures exceptions in multithreaded contexts.
Runtime checks use `std::runtime_error` for exceptional conditions.
Assertions are used extensively in debug builds for invariant checking.
`check` parameter in utility functions controls exception throwing behavior.

**Template Usage:**
Heavy use of template metaprogramming with type traits.
Concepts and `requires` clauses enforce type constraints (C++20).
SFINAE is applied through `std::enable_if_t` and similar constructs.
Compile-time computation via `constexpr` and `static_assert`.
Variadic templates for generic benchmarking and callable wrapping.
Custom type trait utilities in `utils/modules/type_traits.cppm`.

**Concurrency:**
Thread-safe exception handling via `ExceptionSaver`.
Atomic operations for thread-safe counters.
Thread-local storage for random number generators.
Mutex-free designs where possible.

**Performance Practices:**
Move semantics reduce unnecessary copies.
Templates enable zero-cost abstractions.
`[[nodiscard]]` attributes prevent ignoring return values.
`noexcept` specifications enable compiler optimizations.
Inline functions for small utility operations.
Cache-aware data structure design (implicit in cache problem solutions).

---

## 7. Key Files & Entrypoints

**Main Entrypoint:**
Each problem typically has its own CLI entrypoint implemented as a C++ module in `src/<problem>/cli.cppm`.
These executables read from stdin and write solutions to stdout.
Some targets instead define `main` in a different module file (for example, `src/valid_parenthesis/solution.cppm`),
so always consult the corresponding `CMakeLists.txt` or target configuration to locate the actual entrypoint.
Example: `src/lru_cache/cli.cppm`, `src/missing_numbers/cli.cppm`, `src/valid_parenthesis/solution.cppm`.

**Build Configuration:**
`CMakeLists.txt` - Root CMake configuration defining project options and subdirectories.
`CMakePresets.json` - Includes compiler-specific preset configurations.
`cmake/presets/gcc.json` - GCC compiler presets with various build types (debug, release, sanitizers).
`cmake/presets/llvm.json` - LLVM/Clang compiler presets.
`cmake/modules/CodingStandards.cmake` - Defines formatting and linting functions.
`cmake/modules/Testing.cmake` - Configures test execution and code coverage.
`cmake/modules/Installing.cmake` - Handles installation and packaging logic.

**Configuration Files:**
`.clang-format` - Code formatting rules (120 character line limit).
`.clang-tidy` - Static analysis rules with comprehensive check list.
`.cmake-format.py` - CMake file formatting configuration.
`.prettierrc.json` - JSON/YAML/Markdown formatting rules.
`.yamllint.yaml` - YAML linting configuration.
`.markdownlint.yaml` - Markdown linting rules.

**CI/CD Pipeline:**
`.github/workflows/workflow.yml` - Main CI/CD workflow.
Runs on push to main and pull request events.
Uses Nix for reproducible builds via `cachix/install-nix-action`.
Matrix strategy tests 8 different configurations (GCC/LLVM × debug/release × sanitizers);
two GCC sanitizer presets are temporarily disabled pending a GCC 16 upgrade (module ICEs).

---

## 8. Development & Testing Workflow

**Local Development Setup:**
The recommended approach uses Nix for dependency management:

```bash
nix develop
```

This command enters a development shell with all dependencies pre-installed.
For manual CMake configuration without Nix, ensure GCC or Clang, CMake, Ninja, and Boost are available.

**Building:**
Using presets (recommended):

```bash
cmake -B build --preset gcc_release
cmake --build build --parallel
```

Without presets (minimal):

```bash
cmake -B build
cmake --build build --parallel
```

The build system supports various presets: `gcc_debug`, `gcc_release`, `gcc_debug_san`, `gcc_debug_valgrind`, `gcc_release_tsan`, and LLVM equivalents.

**Testing:**
Run all tests with CTest:

```bash
ctest --test-dir build --parallel --verbose
```

Tests include unit tests for each problem solution.
Test executables are named `<problem>_tests`.

**Static Analysis:**
clang-tidy is activated when building with the `LINT=ON` option.
CMake configuration automatically applies clang-tidy to all targets.
Linting tests are added via `add_lint_test()` CMake function.
Linters include `cmake-lint`, `yamllint`, and `typos`.

**Sanitizers:**
AddressSanitizer and UndefinedBehaviorSanitizer: Use `gcc_debug_san` or `llvm_debug_san` presets.
ThreadSanitizer: Use `gcc_release_tsan` or `llvm_release_tsan` presets.
Valgrind: Use `gcc_debug_valgrind` or `llvm_debug_valgrind` presets.
Sanitizer flags are applied via `CMAKE_CXX_FLAGS` in presets.

**Code Coverage:**
Enabled via the `CODE_COVERAGE=ON` CMake option.
GCC uses gcovr for coverage reporting.
Clang uses llvm-profdata and llvm-cov for coverage analysis.
Coverage threshold is set to 70% (line coverage) in the root CMakeLists.txt.
Coverage reports are generated during test execution.

**Formatting:**
Format all source files:

```bash
cmake --build build --target format
```

This target runs `clang-format`, `cmake-format`, and `prettier` on all relevant files.
CI enforces that code is formatted by checking for git diffs after formatting.

**CI/CD Process:**
GitHub Actions workflow runs on every push and pull request.
Three jobs: `build` (Nix package build plus `nix flake check`), `test` (matrix of 8 configurations),
and `format` (a single format-and-diff check).
Tests include building, running unit tests, installation verification, and package creation.
The Nix store and ccache are cached between runs to speed up compilation.

---

## 9. Specific Instructions for AI Collaboration

**Contribution Guidelines:**
Follow the existing code formatting rules enforced by `.clang-format`.
Ensure all new code compiles without warnings when `WARNINGS=ON`.
Add unit tests for new problem solutions or utilities.
Use C++23 features where appropriate and consistent with existing code.
Maintain the modular architecture with clear problem separation.
Update CMakeLists.txt when adding new source files or dependencies.

**Security Considerations:**
Avoid raw pointers; prefer RAII and smart pointers.
Do not hardcode sensitive information or credentials.
Use bounds-checked container access where performance allows.
Enable sanitizers during development to catch memory errors.
Validate external input in CLI programs.

**Dependency Policy:**
Dependencies are managed exclusively through the Nix flake.
To add a new dependency, update `flake.nix` in the appropriate section (buildInputs, nativeBuildInputs).
Boost is the only external library dependency; prefer standard library where possible.
Avoid introducing additional external dependencies without strong justification.

**Commit Messages:**
Commit messages follow a mixed style with some conventional commit patterns.
Recent commits use prefixes like `refactor:`, `chore:`, `fix:`, `docs:`.
Subject line should be concise and descriptive.
Breaking changes should be clearly indicated.
Example: `refactor(project): modernize by usage of std::format`.

**C++ Standard and Features:**
Target C++23 as specified in CMake configurations (`cxx_std_23`).
Use C++20 modules for new code organization.
Prefer modern standard library features (`std::format`, `std::ranges`, concepts).
Apply `constexpr` for compile-time evaluation where beneficial.
Use structured bindings, if-init statements, and other modern syntax.

**Testing Requirements:**
All new features must include unit tests using Boost.Test.
Tests should be added in a `tests/` subdirectory within each problem module.
Use the `add_tests()` CMake function to register tests.
Ensure tests pass with sanitizers enabled.
Code coverage should not decrease significantly with new additions.

**Build System Conventions:**
Each new problem module must have its own `CMakeLists.txt`.
Use `target_allow_coding_standards()` to apply warnings and clang-tidy.
Use `target_allow_instrumentation()` to enable code coverage.
Follow the pattern of existing modules for consistency.
Update `src/CMakeLists.txt` to include new subdirectories.

**Inference Confidence:**
High confidence: C++ standard (C++23), build system (CMake), Nix-based workflow, architectural patterns, naming conventions.
Medium confidence: Some advanced template patterns might be project-specific innovations.
Low confidence: Long-term roadmap and future dependency changes are not documented.

## 10. IMPORTANT NOTES

1. This project uses `nix`.
   All commands should be done in `nix` devshells, i.e. `nix develop --command ...`.
   For example, if user says that to do something you need to run `cmake ...`, that automatically means
   that it should be run in `nix develop --command cmake ...`.
   `git` commands and regular file operations usually can be done without devshell.
2. To test your changes project always use `gcc_debug` and `llvm_debug` presets
   UNLESS you was explicitly told to test with some specific preset
   OR you can reason that some specific preset is required.

   ```bash
   cmake --preset gcc_debug
   cmake --build build/gcc_debug --parallel
   ctest --test-dir build/gcc_debug

   cmake --preset llvm_debug
   cmake --build build/llvm_debug --parallel
   ctest --test-dir build/llvm_debug

   # Also run format!
   cmake --build build/llvm_debug --target format
   ```

   All of this is of course should be inside nix devshell.

3. If user asks to deal with lint errors, you need to use significantly more expensive `llvm_debug_san` preset.

   This preset includes `clang-tidy` checks as well as `yamllint`, `typos` and `cmakelint`.

   ```bash
   cmake --preset llvm_debug_san
   cmake --build build/llvm_debug_san --parallel
   ctest --test-dir build/llvm_debug_san
   # Running formatter in case of lint errors is also a good idea.
   cmake --build build/llvm_debug_san --target format
   ```

   Or, alternatively if you need only `clang-tidy` you can run it directly.
   Though make sure that `compile_commands.json` was last generated with `llvm`-based preset and that preset it built.

   ```bash
   git ls-files "*.h" "*.hpp" "*.cpp" "*.cppm" | xargs --max-args 1 --max-procs $(nproc) clang-tidy
   ```

   If you know specific files to lint, then you can specify them instead of `git ls-files`:

   ```bash
   echo src/path/to/file | xargs --max-args 1 --max-procs $(nproc) clang-tidy
   ```

---

**Project Repository:**
<https://github.com/rudenkornk/cpp_contests>

**License:**
MIT License (Copyright 2020-2026 Nikita Rudenko)

**Maintainer Contact:**
<rudenkornk@gmail.com>
