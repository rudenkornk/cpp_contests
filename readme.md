# C++ contests
[![GitHub Actions Status](https://github.com/rudenkornk/cpp_contests/actions/workflows/workflow.yml/badge.svg)](https://github.com/rudenkornk/cpp_contests/actions)

Repository with one-file solutions for different C++ problems

## Testing Prerequisites
### Option 1: Use docker container interactively:
```bash
make cpp_contests_container
docker attach cpp_contests_container
```
### Option 2: Send commands to docker container:
```bash
make in_docker COMMAND=<command>
```

### Option 3: config your system with provided scripts
Config your system using provided scripts from docker repo:
[Linux](https://github.com/rudenkornk/docker_cpp#3-use-scripts-from-this-repository-to-setup-your-own-system),
[Windows](https://github.com/rudenkornk/docker_cpp_windows/#2-use-scripts-from-this-repository-to-setup-your-own-system)

## Test
### Option 1: Just use cmake
```bash
cmake -B build
cmake --build build
ctest --test-dir build
```

### Option 2: Use make wrapper with preconfigured recommended settings
```bash
make config
make build
make test
```
