# C++ contests

Repository with solutions for different C++ educational problems.

## Prerequisites

The project uses relatively standard set of `C++` tools, which include:

1. Recent compiler (assumed `gcc` or `clang`).
1. `cmake` as a build system.
1. Any `cmake` backend (`ninja` assumed).
1. `Boost` and `fmt` libraries.

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
cmake -S . -B build --preset gcc_release
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
cmake -S build/test_install -B build/test_install/build \
    --preset gcc_release -Dcpp_contests_DIR="$(realpath build/install/lib*/cmake/cpp_contests)"
cmake --build build/test_install/build
cd build && cpack -C CPackConfig.cmake
```
