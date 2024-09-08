# Directories
SRC_D  := ./src
OBJ_D  := ./objects
EXAMPLE_D := ./example_files
INC    := ./include
BIN_D  := ./bin
LIB_D  := ./lib

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -g -I$(INC) -fPIC -Wno-unused-function $(TEMP_CFLAGS)
LIB := -L$(LIB_D) -lgdslib -lm

# Files
C_FILES := $(wildcard $(SRC_D)/*.c)
H_FILES := $(wildcard $(INC)/*.h)
EXAMPLE_FILES := $(wildcard $(EXAMPLE_D)/*.c)

# Binaries for examples
BINARIES := $(BIN_D)/stack $(BIN_D)/ringbuffer $(BIN_D)/queue $(BIN_D)/pqueue $(BIN_D)/linkedlist $(BIN_D)/arraylist $(BIN_D)/arraystack $(BIN_D)/tree

# Phony targets
.PHONY: all clean test run

# Default target
all: lib $(BINARIES)

# Build example binaries using pattern rules
$(BIN_D)/%: $(EXAMPLE_D)/example_g%.c lib
	$(CC) $(CFLAGS) $< $(LIB) -o $@

# Test with sanitizers enabled
test:
	$(MAKE) TEMP_CFLAGS="-fsanitize=address -g3" all

# Compile and archive the static library
lib: obj
	ar rcs $(LIB_D)/libgdslib.a $(wildcard $(OBJ_D)/*.o)

# Compile object files
obj: $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) -c $(C_FILES)
	@echo "\n"
	@mkdir -p $(OBJ_D) $(BIN_D) $(LIB_D)
	@mv *.o $(OBJ_D)/

# Clean the build
clean:
	rm -rf $(OBJ_D)/* $(LIB_D)/* $(BIN_D)/*

# Run a specific example with Valgrind
run: all
	@valgrind ./bin/ring

