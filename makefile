# Name: Almog Hakak, ID: 211825229

# # Directories
# SRC_DIR = src
# INC_DIR = headers
# BIN_DIR = bin

# # Files
# EXEC_FILE = assembler
# C_FILES = $(wildcard $(SRC_DIR)/*.c)
# H_FILES = $(wildcard $(INC_DIR)/*.h)

# # Object files
# O_FILES = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(C_FILES))

# # Targets
# all: $(BIN_DIR) $(EXEC_FILE)

# $(EXEC_FILE): $(O_FILES)
# 	gcc -Wall -ansi -pedantic $(O_FILES) -o $(EXEC_FILE)

# $(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(H_FILES)
# 	gcc -Wall -ansi -pedantic -I$(INC_DIR) -c -o $@ $<

# $(BIN_DIR):
# 	mkdir -p $(BIN_DIR)

# clean:
# 	rm -f $(BIN_DIR)/*.o $(EXEC_FILE)


# Compiler and flags
CFLAGS = -Wall -ansi -pedantic -Iheaders

# Directories
SRC_DIR = src
OBJ_DIR = obj
HDR_DIR = headers

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)                  # List of all .c files in the src directory.
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)  # List of corresponding .o files in the obj directory.

# Output executable
TARGET = assembler                                      # The name of the final executable.

# Rules
all: $(TARGET)

# Rule to build the final executable
$(TARGET): $(OBJ_FILES)
	gcc -o $@ $^

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Phony targets (Declares all and clean as phony targets)
.PHONY: all clean
