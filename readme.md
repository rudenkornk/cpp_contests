# C++ contests
[![GitHub Actions Status](https://github.com/rudenkornk/cpp_contests/actions/workflows/workflow.yml/badge.svg)](https://github.com/rudenkornk/cpp_contests/actions)

Repository with one-file solutions for different C++ problems

## Prerequisites
### Option 1: Use docker container interactively
```bash
make container
docker attach cpp_contests_container
```

### Option 2: Send commands to docker container
```bash
make in_docker COMMAND=<command>
```

### Option 3: config your system
Basic requirements include recent versions of a `C++ compiler`, `CMake` and `Conan`.

You may also want to configure conan profiles, in this case refer to scripts, which were used to create docker images:
[Linux](https://github.com/rudenkornk/docker_cpp#3-use-scripts-from-this-repository-to-setup-your-own-system),
[Windows](https://github.com/rudenkornk/docker_cpp_windows/#2-use-scripts-from-this-repository-to-setup-your-own-system)

## Config, build, test and install
### Option 1: Use make wrapper with preconfigured recommended settings
```bash
make conan
make config
make build
make test
make install
make test_install
make package
```
Note that when using docker container non-interactively there is a shortcut for these makefile targets:
```bash
make in_docker TARGET=<makefile target>
# e.g.:
make in_docker TARGET=conan
make in_docker TARGET=config
# <...> etc.
```


### Option 2: Manually use Conan + CMake
```bash
conan install --build missing --install-folder build scripts/conanfile.py
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build build
ctest --test-dir build
cmake --install build --prefix build/install/
cd build && cpack -C CPackConfig.cmake
```

