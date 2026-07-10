# Build the PocketBook app with the SDK toolchain or run the host tests.
#
#   make                build the PocketBook app (requires PBSDK)
#   make tests          run the host logic tests from tests/
#   make clean          remove build outputs

PBSDK ?=
TOOLCHAIN ?= $(PBSDK)/share/cmake/arm_conf.cmake
BUILD_DIR := build

.PHONY: all app tests clean

all: app

app:
	@if [ -z "$(PBSDK)" ]; then \
		echo "Please export PBSDK=/path/to/SDK_6.3.0 before building." >&2; \
		exit 1; \
	fi
	@if [ ! -f "$(TOOLCHAIN)" ]; then \
		echo "PocketBook toolchain not found at $(TOOLCHAIN)." >&2; \
		exit 1; \
	fi
	@mkdir -p $(BUILD_DIR)
	@cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE="$(TOOLCHAIN)"
	@cmake --build $(BUILD_DIR)

tests:
	$(MAKE) -C tests

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C tests clean
