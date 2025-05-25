CC = gcc
LIB_DIR := include
BUILD_DIR := build

SRC = src/board.c \
       src/game_logic.c \
       src/input.c \
       src/game_flow.c \
       src/main.c

OBJ := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRC))

TARGET := reversi

CFLAGS := -Wall -Wextra -c -I$(LIB_DIR)

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	@echo "Created build directory: $(BUILD_DIR)"

$(TARGET): $(OBJ)
	@echo "Linking object files into $(TARGET)..."
	$(CC) $(OBJ) -o $@
	@echo "Build complete. Executable: ./$(TARGET)"

$(BUILD_DIR)/%.o: src/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning up buid files..."
	@rm -f $(OBJ)
	@rm -rf $(BUILD_DIR)
	@echo "Clean up complete."
