# C++ contests

Repository with solutions for different C++ educational problems.

## Config, build, test and install

### Option 1: Use container with all the required packages installed

**Requirements:** `podman >= 3.4.4` or `docker >= 24.0.1`

```bash
podman run --interactive --tty --detach \
  --env "TERM=xterm-256color" `# colored terminal` \
  --mount type=bind,source="$(pwd)",target="$(pwd)" `# mount your repo` \
  --mount type=bind,source="$HOME/.cache",target="$HOME/.cache" `# mount cache` \
  --name cpp \
  --userns keep-id `# keeps your non-root username` \
  --workdir "$HOME" `# podman sets homedir to the workdir for some reason` \
  ghcr.io/rudenkornk/cpp_ubuntu:22.0.17
podman exec --user root cpp bash -c "chown $(id --user):$(id --group) $HOME"
podman exec --workdir "$(pwd)" --interactive --tty cpp bash
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -r requirements.txt
```

```bash
cmake -S . -B build --preset gcc_release
cmake --build build
ctest --test-dir build
cmake --install build --prefix build/install
```

### Option 2: Manually config your system

**Requirements:** deterministic list of requirements exists, but not specified.
Aside from recent compiler and `CMake` versions there are some optional dependencies:

1. `vcpkg` for the package management, which path should be added to the `VCPKG_INSTALLATION_ROOT` environment variable.
   You most likely want to install this dependency, since it allows to conveniently manage C++ requirements such as `boost` or `fmt`.
   Required for project's CMake presets.
1. `ninja` for project's CMake presets (but you can inherit your local presets and choose another build system).
1. `ccache` for compiler cache. Required for project's CMake presets on Linux (but you can inherit your own local presets and disable `ccache`).
1. `python` for `lit` tests, gcc-based code coverage, `cmake` and `yaml` linting.
1. `valgrind` for valgrind tests.
1. `nodejs` for `prettier` formatter.

For the full dependencies specification you can refer to the [container description](https://github.com/rudenkornk/cpp_image).

Note: `python` and `venv` is used here to get `llvm-lit`, `gcovr`, `cmakelang` and `yamllint`.
If you are not going to run lit tests, gcc-based code coverage or lint code, you can omit setting up virtual env:

```bash
python3 -m venv .venv
source .venv/bin/activate
python3 -m pip install -r requirements.txt
```

```bash
cmake -S . -B build --preset gcc_release
cmake --build build
ctest --test-dir build
cmake --install build --prefix build/install
```

## Format & Lint

Formatting and linting is sensitive to the tools versions, so it is recommended (but not required) to run these commands inside container.

```bash
make format
make lint
```

## Test installation

Besides unit and lit tests you can also check the installation integrity.
`vcpkg` (and `VCPKG_INSTALLATION_ROOT` variable) here become required dependency.

```bash
cmake -S build/test_install -B build/test_install/build \
    --preset gcc_release -Dcpp_contests_DIR="$(realpath build/install/lib/cmake/cpp_contests)"
cmake --build build/test_install/build
cd build && cpack -C CPackConfig.cmake
```
