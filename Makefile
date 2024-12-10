FLAGS        := -Wall -Wextra -std=c99 -g
CC        	:= gcc
SRC_DIRS    := ./src
BUILD_DIR    := ./build
SRCS        := $(shell find $(SRC_DIRS) -name '*.c')
OBJS        := $(SRCS:%=$(BUILD_DIR)/%.o)
TARGET        := ./sfl

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

build: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARGET)

run_sfl:
	$(TARGET)

clean:
	rm -r $(BUILD_DIR)

all: build run_sfl clean