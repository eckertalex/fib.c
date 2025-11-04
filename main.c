#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIB_MAX_N 92
#define FIB_MIN_N (-FIB_MAX_N)

typedef enum FibStatus {
    FIB_SUCCESS,
    FIB_NULL_PTR,
    FIB_OUT_OF_RANGE,
} fib_status_t;

fib_status_t fib(int64_t n, int64_t *out) {
    if (out == NULL) {
        return FIB_NULL_PTR;
    }

    // Handle negative indices: F(-n) = (-1)^(n+1) * F(n)
    if (n < FIB_MIN_N || n > FIB_MAX_N) {
        return FIB_OUT_OF_RANGE;
    }
    bool negate = false;
    if (n < 0) {
        n = -n;
        negate = (n % 2 == 0);
    }

    if (n == 0) {
        *out = 0;
        return FIB_SUCCESS;
    }

    if (n == 1) {
        *out = 1;
        return FIB_SUCCESS;
    }

    uint64_t a = 0, b = 1;
    for (size_t i = 2; i <= (uint64_t)n; i++) {
        uint64_t next = a + b;
        a = b;
        b = next;
    }

    *out = negate ? -(int64_t)b : (int64_t)b;
    return FIB_SUCCESS;
}

/* Advances and returns a pointer past leading whitespace per isspace().
 * Returns NULL if s is NULL. Does not modify the input string.
 */
const char *skip_whitespace(const char *s) {
    if (s == NULL) {
        return NULL;
    }

    while (isspace((unsigned char)*s)) {
        s++;
    }

    return s;
}

typedef enum ParseStatus {
    PARSE_SUCCESS,
    PARSE_NULL_PTR,
    PARSE_EMPTY_STRING,
    PARSE_NO_DIGITS,
    PARSE_OUT_OF_RANGE,
    PARSE_TRAILING_CHARS
} parse_status_t;

/* Parse a string to int64_t with comprehensive error checking.
 * Skips leading whitespace, allows trailing whitespace, detects overflow.
 * Returns PARSE_SUCCESS on success with result written to *out.
 */
parse_status_t parse_int64(const char *s, int64_t *out) {
    if (s == NULL || out == NULL) {
        return PARSE_NULL_PTR;
    }

    s = skip_whitespace(s);
    if (*s == '\0') {
        return PARSE_EMPTY_STRING;
    }

    errno = 0;
    char *end = NULL;
    long long val = strtoll(s, &end, 10);

    if (s == end) {
        return PARSE_NO_DIGITS;
    }

    if (errno == ERANGE || val > (long long)INT64_MAX || val < (long long)INT64_MIN) {
        return PARSE_OUT_OF_RANGE;
    }

    if (*end != '\0') {
        const char *t = skip_whitespace(end);
        if (*t != '\0') {
            return PARSE_TRAILING_CHARS;
        }
    }

    *out = (int64_t)val;
    return PARSE_SUCCESS;
}

typedef enum ReadStatus {
    READ_SUCCESS,
    READ_EOF,
    READ_ERROR,
    READ_TRUNCATED,
} read_status_t;

/* Read a line from stdin into buf.
 * Clears excess input if line exceeds buffer size.
 * Returns READ_SUCCESS, READ_TRUNCATED, READ_EOF, or READ_ERROR.
 */
read_status_t read_line(char *buf, size_t size) {
    if (!fgets(buf, size, stdin)) {
        if (feof(stdin)) {
            return READ_EOF;
        }
        return READ_ERROR;
    }

    // Clear remaining input if buffer was filled without newline
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] != '\n') {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return READ_TRUNCATED;
    }

    return READ_SUCCESS;
}

static const char *get_parse_error(parse_status_t err) {
    switch (err) {
        case PARSE_NULL_PTR:
            return "Panic: Received null pointer.";
        case PARSE_EMPTY_STRING:
            return "Empty string. Please enter a number.";
        case PARSE_NO_DIGITS:
            return "No valid digits found.";
        case PARSE_OUT_OF_RANGE:
            return "Number too large to parse.";
        case PARSE_TRAILING_CHARS:
            return "Invalid characters after number.";
        case PARSE_SUCCESS:
            return NULL;
        default:
            return "Panic: Unhandled error.";
    }
}

static const char *get_fib_error(fib_status_t err) {
    switch (err) {
        case FIB_NULL_PTR:
            return "Panic: Received null pointer.";
        case FIB_OUT_OF_RANGE:
            return NULL;  // Formatted in main: needs FIB_MIN_N/FIB_MAX_N
        case FIB_SUCCESS:
            return NULL;
        default:
            return "Panic: Unhandled error.";
    }
}

int main(void) {
    char buf[32];

    while (true) {
        printf(">> ");
        read_status_t read_status = read_line(buf, sizeof(buf));
        if (read_status == READ_EOF) {
            printf("\n");
            return 0;
        }
        if (read_status == READ_ERROR) {
            fprintf(stderr, "Error reading input.\n");
            return 1;
        }
        if (read_status == READ_TRUNCATED) {
            fprintf(stderr, "Warning: Input too long, truncated to %zu characters.\n", sizeof(buf) - 1);
        }

        if (buf[0] == 'q') {
            return 0;
        }

        int64_t parsed_input = 0;
        parse_status_t parse_status = parse_int64(buf, &parsed_input);
        if (parse_status != PARSE_SUCCESS) {
            fprintf(stderr, "%s\n", get_parse_error(parse_status));
            continue;
        }

        int64_t fib_result = 0;
        fib_status_t fib_status = fib(parsed_input, &fib_result);
        if (fib_status != FIB_SUCCESS) {
            const char *err_msg = get_fib_error(fib_status);
            if (err_msg != NULL) {
                fprintf(stderr, "%s\n", err_msg);
            } else if (fib_status == FIB_OUT_OF_RANGE) {
                fprintf(stderr, "Number out of range (min=%d, max=%d).\n", FIB_MIN_N, FIB_MAX_N);
            }
            continue;
        }

        printf("fib(%" PRId64 ") = %" PRId64 "\n", parsed_input, fib_result);
    }
}
