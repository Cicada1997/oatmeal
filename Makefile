INC_DIR   := include
COM_DIR   := src/components/*/*

# Compiler settings
CXX       := g++
CXXFLAGS  := -Wall -Wextra -I$(INC_DIR) -I$(COM_DIR)

# Directory layout
SRC_DIR   := src
OBJ_DIR   := obj
BIN_DIR   := bin

# Libraries and flags
PKG_LIBS = sdl2 fmt

LIB_CFLAGS = $(shell pkg-config --cflags $(PKG_LIBS))
LIB_LDFLAGS = $(shell pkg-config --libs $(PKG_LIBS))

# Source files and corresponding object files
SOURCES   := $(wildcard $(SRC_DIR)/*.cc)
OBJECTS   := $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SOURCES))

# Final executable
EXECUTABLE := $(BIN_DIR)/main

# Default target
all: $(EXECUTABLE)
	#
# Compile step: ensure obj directory exists first
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(LIB_CFLAGS) -c $< -o $@

# Link step: ensure bin directory exists first
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LIB_LDFLAGS)

# Clean up generated files
clean:
	@rm -rf $(OBJ_DIR)/*.o $(EXECUTABLE)

.PHONY: all clean

