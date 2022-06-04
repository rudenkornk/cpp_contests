# This makefile only has helper targets, it is not meant to handle dependencies --- this is cmake's job
SHELL = /usr/bin/env bash

PROJECT_NAME := cpp_contests
TOOLCHAIN ?= gcc
BUILD_DIR ?= build
GENERATOR ?= Ninja Multi-Config
PARALLEL ?= ON
LIT ?= ON
CODE_COVERAGE ?= ON
VALGRIND ?= OFF
BUILD_TYPE ?= Debug

CORES := $(shell nproc --all)
PARALLEL_OPTION := $(shell [[ "$(PARALLEL)" == "ON" ]] && echo "--parallel $(CORES)")
CC := $(shell \
        if [[ "$(TOOLCHAIN)" == "gcc" ]]; then \
          echo "gcc"; \
        elif [[ "$(TOOLCHAIN)" == "llvm" ]]; then \
          echo "clang"; \
        else \
          echo "unknown_toolchain"; \
        fi \
       )
CXX := $(shell \
        if [[ "$(TOOLCHAIN)" == "gcc" ]]; then \
          echo "g++"; \
        elif [[ "$(TOOLCHAIN)" == "llvm" ]]; then \
          echo "clang++"; \
        else \
          echo "unknown_toolchain"; \
        fi \
       )
MAKEFILE := $(firstword $(MAKEFILE_LIST))


.PHONY: config
config:
	CC=$(CC) CXX=$(CXX) cmake -S . -B "$(BUILD_DIR)" -G "$(GENERATOR)" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DPARALLEL_TESTING=$(PARALLEL) -DLIT=$(LIT) -DVALGRIND=$(VALGRIND) -DCODE_COVERAGE=$(CODE_COVERAGE)

.PHONY: build
build:
	cmake --build "$(BUILD_DIR)" --config $(BUILD_TYPE) $(PARALLEL_OPTION)

.PHONY: test
test:
	ctest --build-config $(BUILD_TYPE) --test-dir "$(BUILD_DIR)" $(PARALLEL_OPTION) --verbose

.PHONY: install
install: # not implemented yet

###################### docker support ######################
KEEP_CI_USER_SUDO ?= false
KEEP_CI_USER_SUDO := $(KEEP_CI_USER_SUDO)
TARGET ?=
DOCKER_IMAGE_TAG ?= rudenkornk/docker_cpp:0.1.7
DOCKER_IMAGE_TAG := $(DOCKER_IMAGE_TAG)
DOCKER_CONTAINER_NAME ?= $(PROJECT_NAME)_container
DOCKER_CONTAINER_NAME := $(DOCKER_CONTAINER_NAME)
DOCKER_CONTAINER := $(BUILD_DIR)/$(DOCKER_CONTAINER_NAME)

IF_DOCKERD_UP := command -v docker &> /dev/null && pidof dockerd &> /dev/null

DOCKER_CONTAINER_ID := $(shell $(IF_DOCKERD_UP) && docker container ls --quiet --all --filter name=^/$(DOCKER_CONTAINER_NAME)$)
DOCKER_CONTAINER_STATE := $(shell $(IF_DOCKERD_UP) && docker container ls --format {{.State}} --all --filter name=^/$(DOCKER_CONTAINER_NAME)$)
DOCKER_CONTAINER_RUN_STATUS := $(shell [[ "$(DOCKER_CONTAINER_STATE)" != "running" ]] && echo "$(DOCKER_CONTAINER)_not_running")
.PHONY: $(DOCKER_CONTAINER)_not_running
$(DOCKER_CONTAINER): $(DOCKER_CONTAINER_RUN_STATUS)
ifneq ($(DOCKER_CONTAINER_ID),)
	docker container rename $(DOCKER_CONTAINER_NAME) $(DOCKER_CONTAINER_NAME)_$(DOCKER_CONTAINER_ID)
endif
	docker run --interactive --tty --detach \
		--env BUILD_DIR="$(BUILD_DIR)" \
		--env GENERATOR="$(GENERATOR)" \
		--env BUILD_TYPE="$(BUILD_TYPE)" \
		--env PARALLEL="$(PARALLEL)" \
		--env LIT="$(PARALLEL)" \
		--env CODE_COVERAGE="$(CODE_COVERAGE)" \
		--env VALGRIND="$(VALGRIND)" \
		--env TOOLCHAIN="$(TOOLCHAIN)" \
		--env KEEP_CI_USER_SUDO="$(KEEP_CI_USER_SUDO)" \
		--env CI_UID="$$(id --user)" --env CI_GID="$$(id --group)" \
		--env "TERM=xterm-256color" \
		--user ci_user \
		--name $(DOCKER_CONTAINER_NAME) \
		--mount type=bind,source="$$(pwd)",target=/home/repo \
		$(DOCKER_IMAGE_TAG)
	sleep 1
	mkdir --parents $(BUILD_DIR) && touch $@

.PHONY: $(DOCKER_CONTAINER_NAME)
$(DOCKER_CONTAINER_NAME): $(DOCKER_CONTAINER)

.PHONY: in_docker
in_docker: $(DOCKER_CONTAINER)
	docker exec $(DOCKER_CONTAINER_NAME) bash -c "make --file $(MAKEFILE) $(TARGET)"

