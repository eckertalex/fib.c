#!/bin/sh
# CC picks the compiler, clang by default
set -eu

cd "$(dirname "$0")"

mkdir -p build
${CC:-clang} -std=c23 -O0 -g -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wundef -Wno-unused-function \
    fib.c -o build/fib
