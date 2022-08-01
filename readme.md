# C++ contests
[![GitHub Actions Status](https://github.com/rudenkornk/cpp_contests/actions/workflows/workflow.yml/badge.svg)](https://github.com/rudenkornk/cpp_contests/actions)

Repository with one-file solutions for different C++ problems

## Config, build, test and install
### Option 1: Send commands to docker container
```bash
make in_docker TARGET=conan
make in_docker TARGET=config
make in_docker TARGET=build
make in_docker TARGET=test
make in_docker TARGET=install
make in_docker TARGET=test_install
make in_docker TARGET=package
```

### Option 2: Use docker container interactively
```bash
make container
docker attach cpp_contests_container
make conan
make config
make build
make test
make install
make test_install
make package
```

### Option 3: Manually config your system
Basic requirements include recent versions of a `C++ compiler`, `CMake` and `Conan`.

`CI` in this repo also uses custom `Conan` profiles, which allow to use different compilers and sanitizers. Although it is not required, you may want to use similar profiles.
In this case refer to scripts, which were used to create docker images:
[Linux](https://github.com/rudenkornk/docker_cpp#3-use-scripts-from-this-repository-to-setup-your-own-system),
[Windows](https://github.com/rudenkornk/docker_cpp_windows/#2-use-scripts-from-this-repository-to-setup-your-own-system)

```bash
conan install --build missing --install-folder build scripts/conanfile.py
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build build
ctest --test-dir build
cmake --install build --prefix build/install/
cd build && cpack -C CPackConfig.cmake
```

