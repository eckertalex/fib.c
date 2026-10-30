# fib.c

A small command-line Fibonacci calculator in C, written as a learning project.
It runs an interactive prompt and computes F(n) for indices -92 to 92, the
largest range that fits in a signed 64-bit integer. Negative indices use
F(-n) = (-1)^(n+1) F(n).

```sh
./build/fib
>> 10
fib(10) = 55
>> q
```

## Build

`build.sh` builds into `build/`.

```sh
./build.sh            # build/fib
CC=gcc ./build.sh     # clang is the default
```
