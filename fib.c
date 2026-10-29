#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define internal static
#define global static
#define local_persist static

typedef int64_t i64;
typedef uint64_t u64;

constexpr i64 FIB_MAX = 92;

internal bool read_line(char *buffer, int size) {
    if(!fgets(buffer, size, stdin)) {
        return false;
    }

    size_t len = strlen(buffer);
    if(len > 0 && buffer[len - 1] != '\n') {
        int c;
        while((c = getchar()) != '\n' && c != EOF);
    }

    return true;
}

internal char *skip_whitespace(char *s) {
    while(isspace((unsigned char)*s)) {
        s++;
    }

    return s;
}

internal bool parse_i64(char *s, i64 *out) {
    s = skip_whitespace(s);

    if(*s == '\0') {
        return false;
    }

    char *end;
    long long value = strtoll(s, &end, 10);

    if(s == end) {
        return false;
    }

    end = skip_whitespace(end);

    if(*end != '\0') {
        return false;
    }

    *out = (i64)value;
    return true;
}

internal i64 fib(i64 n) {
    assert(n >= 0);
    assert(n <= FIB_MAX);

    if(n == 0) {
        return 0;
    }

    i64 a = 0;
    i64 b = 1;

    for(i64 i = 1; i < n; i++) {
        i64 next = a + b;
        a = b;
        b = next;
    }

    return b;
}

internal void test_fib(void) {
    assert(fib(0) == 0);
    assert(fib(1) == 1);
    assert(fib(2) == 1);
    assert(fib(3) == 2);
    assert(fib(10) == 55);
    assert(fib(92) == 7540113804746346429LL);
}

int main(void) {
    test_fib();

    char buffer[32];

    for (;;) {
        printf("> ");

        if(!read_line(buffer, sizeof(buffer))) {
            break;
        }

        if(buffer[0] == 'q') {
            break;
        }

        i64 n;

        if(!parse_i64(buffer, &n)) {
            printf("bad input\n");
            continue;
        }

        if(n < 0 || n > FIB_MAX) {
            printf("out of range (0..%lld)\n", FIB_MAX);
            continue;
        }

        i64 result = fib(n);

        printf("fib(%" PRId64 ") = %" PRId64 "\n", n, result);
    }

    return EXIT_SUCCESS;
}
