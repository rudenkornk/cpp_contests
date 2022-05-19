# This makefile only has helper targets, it is not meant to handle dependencies --- this is cmake's job
SHELL = /usr/bin/env bash

PROJECT_NAME := cpp_contests
BUILD_DIR ?= build
MAKE_BUILD_DIR ?= build_docker
PARALLEL ?= 16
BUILD_TYPE ?= debug
COMMIT ?= HEAD^
CLANG_FORMAT_OPTIONS ?= -i
CLANG_TIDY_OPTIONS ?=
CONFIG_OPTIONS ?=
BUILD_OPTIONS ?=
TEST_OPTIONS ?=

.PHONY: config
config:
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -B $(BUILD_DIR) $(CONFIG_OPTIONS)

.PHONY: build
build:
	cmake --build $(BUILD_DIR) --parallel $(PARALLEL) $(BUILD_OPTIONS)

.PHONY: test
test:
	ctest --test-dir $(BUILD_DIR) --parallel $(PARALLEL) $(TEST_OPTIONS)

.PHONY: clang-format
clang-format:
	git diff -U0 --no-color $(COMMIT) | \
		clang-format-diff -p1 $(CLANG_FORMAT_OPTIONS)

.PHONY: clang-tidy
clang-tidy:
	git diff -U0 --no-color $(COMMIT) | \
		clang-tidy-diff.py -j$(PARALLEL) -p1 -path $(BUILD_DIR) $(CLANG_TIDY_OPTIONS)


###################### docker support ######################
KEEP_CI_USER_SUDO ?= false
KEEP_CI_USER_SUDO := $(KEEP_CI_USER_SUDO)
DOCKER_COMMAND ?=
DOCKER_IMAGE_TAG ?= rudenkornk/docker_cpp:0.1.3
DOCKER_IMAGE_TAG := $(DOCKER_IMAGE_TAG)
DOCKER_CONTAINER_NAME ?= $(PROJECT_NAME)_container
DOCKER_CONTAINER_NAME := $(DOCKER_CONTAINER_NAME)
DOCKER_CONTAINER := $(MAKE_BUILD_DIR)/$(DOCKER_CONTAINER_NAME)

IF_DOCKERD_UP = command -v docker &> /dev/null && pidof dockerd &> /dev/null

DOCKER_CONTAINER_ID = $(shell $(IF_DOCKERD_UP) &> /dev/null && docker container ls --quiet --all --filter name=^/$(DOCKER_CONTAINER_NAME)$)
DOCKER_CONTAINER_STATE = $(shell $(IF_DOCKERD_UP) &> /dev/null && docker container ls --format {{.State}} --all --filter name=^/$(DOCKER_CONTAINER_NAME)$)
DOCKER_CONTAINER_RUN_STATUS = $(shell [[ "$(DOCKER_CONTAINER_STATE)" != "running" ]] && echo "$(DOCKER_CONTAINER)_not_running")
.PHONY: $(DOCKER_CONTAINER)_not_running
$(DOCKER_CONTAINER): $(DOCKER_CONTAINER_RUN_STATUS)
ifneq ($(DOCKER_CONTAINER_ID),)
	docker container rename $(DOCKER_CONTAINER_NAME) $(DOCKER_CONTAINER_NAME)_$(DOCKER_CONTAINER_ID)
endif
	docker run --interactive --tty --detach \
		--user ci_user \
		--env KEEP_CI_USER_SUDO="$(KEEP_CI_USER_SUDO)" \
		--env CI_UID="$$(id --user)" --env CI_GID="$$(id --group)" \
		--env "TERM=xterm-256color" \
		--name $(DOCKER_CONTAINER_NAME) \
		--mount type=bind,source="$$(pwd)",target=/home/repo \
		$(DOCKER_IMAGE_TAG)
	sleep 1
	mkdir --parents $(MAKE_BUILD_DIR) && touch $@

.PHONY: $(DOCKER_CONTAINER_NAME)
$(DOCKER_CONTAINER_NAME): $(DOCKER_CONTAINER)

.PHONY: in_docker
in_docker: $(DOCKER_CONTAINER)
	docker exec $(DOCKER_CONTAINER_NAME) bash -c "$(DOCKER_COMMAND)"

