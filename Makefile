TARGET = fib
CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic
DEBUG_FLAGS = -Og -g -fsanitize=address -fsanitize=undefined
RELEASE_FLAGS = -O2

SRCS = $(wildcard *.c)

all: release

release: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $@ $^

debug: $(TARGET)_debug

$(TARGET)_debug: $(SRCS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(TARGET)_debug

.PHONY: all release debug clean
