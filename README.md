# fib.c

A small command-line Fibonacci calculator in C, written as a learning project.
It runs an interactive prompt and computes F(n) for indices -92 to 92 — the
largest range that fits in a signed 64-bit integer. Negative indices use
F(-n) = (-1)^(n+1) F(n).

```sh
./build/fib
>> 10
fib(10) = 55
>> q
```

## Build

`build.sh` builds into `build/`. No make, no build system.

```sh
./build.sh            # debug, with AddressSanitizer and UBSan
./build.sh release    # optimized (-O2)
./build.sh run        # build and run in one step
./build.sh gcc        # clang is the default, also takes gcc or cc
```
