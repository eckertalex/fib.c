TARGET = fib
CC = cc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic
DEBUG_FLAGS = -Og -g -fsanitize=address -fsanitize=undefined
RELEASE_FLAGS = -O2 -DNDEBUG

all: release

release: $(TARGET)

$(TARGET): fib.c
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $@ fib.c

debug: fib.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(TARGET)_debug fib.c

clean:
	rm -f $(TARGET) $(TARGET)_debug *.dSYM

.PHONY: all release debug clean
