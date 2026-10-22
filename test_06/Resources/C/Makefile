# Makefile for the App SW precompiled library (pkg_appsw)
#
# Builds every .c file under src/ and links them into the App SW dynamic
# library, written into ../Build (i.e. Resources/Build, exactly where
# pkg_appsw.mo's LibraryDirectory annotation expects it):
#
#   - Resources/Build/libappsw.so        (Linux, native gcc)
#   - Resources/Build/appsw.dll          (Windows, mingw-w64 cross-compile)
#   - Resources/Build/libappsw.dll.a     (Windows import library, alongside the .dll)
#
# Usage (run from THIS directory, Resources/C):
#   make            # builds for the host platform (auto-detected)
#   make linux      # force a Linux shared library (libappsw.so)
#   make windows    # force a Windows DLL (appsw.dll) via mingw-w64
#   make clean      # remove all build output
#
# NOTE: object files are platform-specific. Run `make clean` before
# switching from `make linux` to `make windows` or vice versa.

CC        ?= gcc
CFLAGS    ?= -std=c99 -Wall -Wextra -O2 -fPIC
SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := ../Build

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

.PHONY: all linux windows clean

ifeq ($(OS),Windows_NT)
all: windows
else
all: linux
endif

linux: $(BUILD_DIR)/libappsw.so

windows: CC := x86_64-w64-mingw32-gcc
windows: $(BUILD_DIR)/appsw.dll

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(BUILD_DIR)/libappsw.so: $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

$(BUILD_DIR)/appsw.dll: $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) -Wl,--out-implib,$(BUILD_DIR)/libappsw.dll.a

clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/libappsw.so $(BUILD_DIR)/appsw.dll $(BUILD_DIR)/libappsw.dll.a
