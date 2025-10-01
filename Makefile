CC := gcc
OFLAGS := -g -O2
CFLAGS := $(OFLAGS) \
		  -Wextra -Wall -Wpedantic \
		  -fsanitize=undefined -fsanitize=address

INCLUDES := -I./src
LIBS = -L./libs/SDL3/build -L./libs/SDL3_image/build -lSDL3 -lSDL3_image

SRC_DIR := ./src
BUILD_DIR := ./build
EXE := $(BUILD_DIR)/a.out

SRC := $(shell find $(SRC_DIR) -type f -name "*.c")
HDR := $(shell find $(SRC_DIR) -type f -name "*.h")
OBJ := $(subst $(SRC_DIR), $(BUILD_DIR), $(SRC:.c=.o))

all: $(OBJ) $(EXE)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "------ Make $(@) ------"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDES) -c $< -o $@

$(EXE) : $(OBJ) | $(BUILD_DIR)
	@echo "------ Make $(EXE) ------"
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(INCLUDES) -o $(EXE) 

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	@echo "------ Running $(EXE) ------"
	$(EXE) $(ARGS)

clean:
	rm -rf $(BUILD_DIR)

format: $(SRC) $(HDR)
	clang-format -i $(SRC) $(HDR)

.PHONY: all run clean format
