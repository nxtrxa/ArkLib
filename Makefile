CC            := clang
CSTD          := -std=c2y

INCLUDE_DIRS  := -Iinclude -Iinclude/core -Iinclude/internals -Iexternal

WFLAGS 		  := -Wall -Wextra -Wpedantic -Wswitch-enum -Wno-import-preprocessor-directive-pedantic                             \
                 -Iexternal -fdefer-ts -fmodules -fmodule-map-file=module.modulemap -Wsizeof-pointer-memaccess -Wshift-overflow	\
		   		 -D_GNU_SOURCE -fblocks -fPIC -Xclang -fexperimental-bounds-safety -fblocks                          \

CFLAGS 		  := $(WFLAGS)
CFLAGS		  += $(INCLUDE_DIRS)

LDFLAGS       := -Wl,-rpath,'$$ORIGIN/arklib' -lBlocksRuntime

DEPFLAGS      := -MMD -MP -MF

# LTO flags (only used in release mode)
LTOFLAGS      := -flto=thin

# Release / Debug extra flags
CFLAGS_REL    := -O3 -march=native $(LTOFLAGS)
CFLAGS_DBG    := -g -O0 -DDEBUG -fno-omit-frame-pointer

SANITIZE_ASAN := -fsanitize=address
SANITIZE_MSAN := -fsanitize=memory -fsanitize-memory-track-origins
SANITIZE_TSAN := -fsanitize=thread

# llvm-ar (required for LTO object archives, safe for normal objects too)
AR            := llvm-ar

SRC_DIR       := src
INCLUDE_DIR   := include
TEST_DIR      := tests
VENDOR_DIR    := external
BLOCKS_DIR    := $(VENDOR_DIR)/BlocksRuntime
OPENGLTV_DIR  := $(VENDOR_DIR)/opengltv

# ------------------------------------------------------------------------------
# Mode detection & directory setup
# ------------------------------------------------------------------------------
ifdef DEBUG
    BUILD_MODE   := debug
    CFLAGS       += $(CFLAGS_DBG)
    BIN_DIR      := bin/debug
    LIB_DIR      := arklib/debug
    TEST_OUT_DIR := tests/out/debug
else ifdef ASAN
    BUILD_MODE   := debug
    CFLAGS       += $(CFLAGS_DBG) $(SANITIZE_ASAN)
    BIN_DIR      := bin/debug/asan
    LIB_DIR      := arklib/debug/asan
    TEST_OUT_DIR := tests/out/debug/asan
else ifdef MSAN
    BUILD_MODE   := debug
    CFLAGS       += $(CFLAGS_DBG) $(SANITIZE_MSAN)
    BIN_DIR      := bin/debug/msan
    LIB_DIR      := arklib/debug/msan
    TEST_OUT_DIR := tests/out/debug/msan
else ifdef TSAN
    BUILD_MODE   := debug
    CFLAGS       += $(CFLAGS_DBG) $(SANITIZE_TSAN)
    BIN_DIR      := bin/debug/tsan
    LIB_DIR      := arklib/debug/tsan
    TEST_OUT_DIR := tests/out/debug/tsan
else ifdef RELEASE
    BUILD_MODE   := release
    CFLAGS       += $(CFLAGS_REL)
    LDFLAGS      += $(LTOFLAGS)   # LTO must also be passed to the linker
    BIN_DIR      := bin/release
    LIB_DIR      := arklib/release
    TEST_OUT_DIR := tests/out/release
else ifdef ASM_DBG
	BUILD_MODE   := debug
    CFLAGS       += $(CFLAGS_DBG) -S
    TEST_OUT_DIR := tests/asm/debug
else ifdef ASM_REL
	BUILD_MODE   := release
    CFLAGS       += $(CFLAGS_REL) -S
    TEST_OUT_DIR := tests/asm/release
else
    # Default to release when no mode is given
    BUILD_MODE   := release
    CFLAGS       += $(CFLAGS_REL)
    LDFLAGS      += $(LTOFLAGS)
    BIN_DIR      := bin/release
    LIB_DIR      := arklib/release
    TEST_OUT_DIR := tests/out/release
endif

# ------------------------------------------------------------------------------
# Files & targets
# ------------------------------------------------------------------------------
STATIC_TARGET := $(LIB_DIR)/libarklib.a
SHARED_TARGET := $(LIB_DIR)/libarklib.so

# Find all source and header files
SRC_FILES     := $(shell find $(SRC_DIR) -type f -name "*.c")
H_FILES       := $(shell find $(INCLUDE_DIR) -type f -name "*.h")
TEST_FILES    := $(shell find $(TEST_DIR) -type f -name "*.c" 2>/dev/null)

# Object files from main sources
OBJ_FILES     := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))

# BlocksRuntime objects (now that BIN_DIR is known)
BLOCK_OBJS    := $(BIN_DIR)/BlocksRuntime/runtime.o $(BIN_DIR)/BlocksRuntime/data.o
OBJ_FILES     += $(BLOCK_OBJS)

# Dependency files (.d) generated alongside .o files
DEP_FILES     := $(OBJ_FILES:.o=.d)

# Targets that are not actual files
.PHONY: all clean install test run release debug

# ------------------------------------------------------------------------------
# Top-level targets
# ------------------------------------------------------------------------------
all: $(STATIC_TARGET) $(SHARED_TARGET)

# Create necessary directories
$(BIN_DIR) $(LIB_DIR) $(TEST_OUT_DIR):
	@mkdir -p $@

# Compile main sources .c → .o (pattern rule)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CSTD) $(CFLAGS) -c $< -o $@ $(DEPFLAGS) $(@:.o=.d)

# Compile BlocksRuntime sources .c → .o
$(BIN_DIR)/BlocksRuntime/%.o: $(BLOCKS_DIR)/%.c | $(BIN_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CSTD) $(CFLAGS) -I$(BLOCKS_DIR) -c $< -o $@ $(DEPFLAGS) $(@:.o=.d)

$(BIN_DIR)/BlocksRuntime/%.o: CFLAGS += -Wno-unused-parameter  -Wno-unused-function

$(BIN_DIR)/opengltv/%.o: $(OPENGLTV_DIR)/%.c | $(BIN_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CSTD) $(CFLAGS) -I$(OPENGLTV_DIR) -c $< -o $@ $(DEPFLAGS) $(@:.o=.d)

$(BIN_DIR)/opengltv/%.o: CFLAGS += -Wno-unused-parameter  -Wno-unused-function

# Static library
$(STATIC_TARGET): $(OBJ_FILES) | $(LIB_DIR)
	$(AR) rcs $@ $^

# Shared library
$(SHARED_TARGET): $(OBJ_FILES) | $(LIB_DIR)
	$(CC) -shared $^ -o $@ $(LDFLAGS)

# Install shared library to system directory (optional)
install: $(SHARED_TARGET)
	@echo "Installing headers to /usr/include/ark..."
	sudo mkdir -p /usr/include/ark
	sudo cp -r $(INCLUDE_DIR)/* /usr/include/ark/
	@echo "Installing libraries to /usr/lib..."
	sudo install -m 644 $(STATIC_TARGET) /usr/lib/
	sudo install -m 755 $(SHARED_TARGET) /usr/lib/
	@echo "Installation complete."

# Include auto-generated dependency files (if they exist)
-include $(DEP_FILES)

# Clean build artifacts (removes ALL modes, not just the current one)
clean:
	rm -rf bin arklib tests/out

# ------------------------------------------------------------------------------
# Generic test runner – compile and execute any test/%.c with current mode
# ------------------------------------------------------------------------------
.PHONY: run_% asm_%

run_%: $(STATIC_TARGET) $(SHARED_TARGET) | $(TEST_OUT_DIR)
	$(CC) $(CSTD) $(CFLAGS) $(TEST_DIR)/$*.c -o $(TEST_OUT_DIR)/$* -L$(LIB_DIR) -larklib $(LDFLAGS)
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH $(TEST_OUT_DIR)/$*

asm_%: | $(TEST_OUT_DIR)
	$(CC) $(CSTD) $(CFLAGS) -S $(TEST_DIR)/$*.c -o $(TEST_OUT_DIR)/$*.s

# ------------------------------------------------------------------------------
# Convenience targets that force a specific build mode and then run a test
# Usage: make debug_<testname>   e.g. make debug_main
#        make release_<testname>
#        make asan_<testname>
#        make msan_<testname>
#        make tsan_<testname>
# ------------------------------------------------------------------------------
.PHONY: debug_% release_% asan_% msan_% tsan_%
dbg_%:
	@$(MAKE) DEBUG=1 run_$*
rel_%:
	@$(MAKE) RELEASE=1 run_$*
asan_%:
	@$(MAKE) ASAN=1 run_$*
msan_%:
	@$(MAKE) MSAN=1 run_$*
tsan_%:
	@$(MAKE) TSAN=1 run_$*
asmd_%:
	@$(MAKE) ASM_DBG=1 asm_$*
asmr_%:
	@$(MAKE) ASM_REL=1 asm_$*

# ------------------------------------------------------------------------------
# Run all tests using the current (default) build mode
# ------------------------------------------------------------------------------
TEST_NAMES := $(notdir $(basename $(TEST_FILES)))
test: all
	@$(MAKE) $(addprefix run_,$(TEST_NAMES))
