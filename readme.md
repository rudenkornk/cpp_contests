# C++ contests

Repository with solutions for different C++ educational problems.

## Prerequisites

The project uses relatively standard set of `C++` tools, which include:

1. Recent compiler (assumed `gcc` or `clang`).
1. `cmake` as a build system.
1. Any `cmake` backend (`ninja` assumed).
1. `Boost` library.

As well as optional dependencies:

1. `ccache` for compiler cache.
1. `llvm-profdata` and `llvm-cov` for code coverage tests with `clang`.
1. `gcovr` for code coverage tests with `gcc`.
1. `valgrind` for any `valgrind` tests.
1. `clang-format`, `cmake-format` and `prettier` for formatting.
1. `clang-tidy`, `cmake-lint`, `yamllint` and `typos` for linting.

For the full dependencies specification you can refer to the [`nix` recipe](./flake.nix).

To smoothly deliver these dependencies the project utilizes [`nix`](https://nixos.org/).
It is recommended to use for local development too:

```bash
nix develop # Enter devshell with all dependencies installed.
```

## Config, build, test and install

The project has several presets preconfigured, see them with `cmake --list-presets`.
All presets require at least some optional dependencies, so for bare minimal build omit their usage.

```bash
cmake -B build --preset gcc_release
cmake --build build --parallel
ctest --test-dir build
cmake --install build --prefix build/install
```

## Format

```bash
cmake --build build --target format
```

## Test installation

Besides general tests you can also check installation integrity.

```bash
cmake -G Ninja -S build/test_install -B build/test_install/build \
    -Dcpp_contests_DIR="$(realpath build/install/lib*/cmake/cpp_contests)"
cmake --build build/test_install/build
cd build && cpack -C CPackConfig.cmake
```

## `import std` (experimental)

C++23 `import std;` was spiked with CMake 4.1.2 (gated behind `CMAKE_EXPERIMENTAL_CXX_IMPORT_STD` plus
`CMAKE_CXX_MODULE_STD=ON`):

- **GCC 15.2 + libstdc++**: builds and runs.
- **Clang 21 (this devshell's libstdc++)**: fails to configure the `std` module; it would need
  `-stdlib=libc++`.

Because the project targets both toolchains, the sources keep using classic global-module-fragment
`#include`s for now.
Once Clang can consume the same `std` module, they can switch to `import std;`.
