Take a solution from a.cpp and format it as another solution in proj_root/src/

1. Copy solution AS IS WITHOUT ANY MODIFICATIONS OR "IMPROVEMENTS" except stated explicitly below.
1. Use modules. Copy this file with cppm extension, add `module;` at the top, and `export module <task>;`
1. Reformat using clang-format rules from the root (just run `cmake --build --target format`).
1. Use min_max_arrays as an example reference for all the cmake stuff, tests, readme, module and namespace.
1. Add namespace cpp_contests where appropriate.
1. Register project in root CMakeLists.txt
1. Use CMakeLists.txt in project SIMILAR to the provided example!
1. Add SIMPLE AND SHORT readme describing the task (original task is given below).
1. Add tests/. Add a single test case FROM THE ORIGINAL TASK. No more.
   Use the same min_max_arrays as an example.
1. Fix clang-tidy issues (run `clang-tidy` manually). Typically you need to fix naming.
   Check clang-tidy by running build with `llvm_debug_san` preset.

   ```bash
   cmake --preset llvm_debug_san
   cmake --build build/llvm_debug_san --parallel
   ```

After that verify that project builds and tests pass. If not, fix the issues.
Make commit.
The original problem:
