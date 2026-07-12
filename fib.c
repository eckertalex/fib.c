#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int64_t s64;
typedef uint64_t u64;

#define FIB_MAX 92

static bool read_line(char *buffer, int size) {
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

static char *skip_whitespace(char *s) {
    while(isspace((unsigned char)*s)) {
        s++;
    }

    return s;
}

static bool parse_s64(char *s, s64 *out) {
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

    *out = (s64)value;
    return true;
}

static s64 fib(s64 n) {
    assert(n >= 0);
    assert(n <= FIB_MAX);

    if(n == 0) {
        return 0;
    }

    s64 a = 0;
    s64 b = 1;

    for(s64 i = 1; i < n; i++) {
        s64 next = a + b;
        a = b;
        b = next;
    }

    return b;
}

static void test_fib(void) {
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

        s64 n;

        if(!parse_s64(buffer, &n)) {
            printf("bad input\n");
            continue;
        }

        if(n < 0 || n > FIB_MAX) {
            printf("out of range (0..%d)\n", FIB_MAX);
            continue;
        }

        s64 result = fib(n);

        printf("fib(%" PRId64 ") = %" PRId64 "\n", n, result);
    }

    return EXIT_SUCCESS;
}
