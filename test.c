#include "test.h"
#include "printf.h"
#include "string.h"
#include "math.h"
#include <stdint.h>

#define ASSERT(expr, num) \
    if (expr) \
        printf("%s #%d: PASSED\n", __func__, num); \
    else \
        printf("%s #%d: FAILED\n", __func__, num); \

void KERNEL_test_assert() {
    ASSERT(0 == 0, 1);
}

void USER_test_assert() {
    ASSERT(0 == 0, 1);
}

void USER_test_string() {
    /* strlen */
    ASSERT(strlen("test") == 4, 1);
    ASSERT(strlen("hello world!") == 12, 2);

    /* strcat */
    char s1[15] = "hello";
    char s2[6] = "world";
    strcat(s1, s2, 15);
    ASSERT(strcmp(s1, "helloworld") == 1, 3);
    ASSERT(strcat(s1, s2, 15) == 0, 4); // too large

    /* strcatc */
    char s3[8] = "hello";
    strcatc(s3, '!', 8);
    ASSERT(strcmp(s3, "hello!") == 1, 5);
    char s4[6] = "hello";
    ASSERT(strcatc(s4, '!', 6) == 0, 6); // too large

    char s5[10] = "hello";
    char s6[10] = "wrld";
    strncpy(s5, s6, 5, 10); // try copying too much
    ASSERT(strcmp(s5, "wrld") == 1, 7);

    /* strcmp */
    ASSERT(strcmp("test", "test") == 1, 8);
    ASSERT(strcmp("test", "testa") == 0, 9);
        ASSERT(strcmp("test", "best") == 0, 10);
}

void USER_test_math() {
    /* digit_to_hex */
    ASSERT(digit_to_hex(12) == 'C', 1);
    ASSERT(digit_to_hex(9) == '9', 2);

    /* hex_to_digit */
    ASSERT(hex_to_digit('B') == 11, 3);
    ASSERT(hex_to_digit('8') == 8, 4);

    /* hex_to_int */
    ASSERT(hex_to_int("7FFFFFFF") == 2147483647, 5);
    ASSERT(hex_to_int("3C5A") == 15450, 6);

    /* int_to_hex */
    char buf[9];
    ASSERT(strcmp(int_to_hex(15, buf), "0000000F") == 1, 7);
    ASSERT(strcmp(int_to_hex(16*16-1, buf), "000000FF") == 1, 8);
}

/* TEST EVERYTHING */

void KERNEL_tests() {
    KERNEL_test_assert();
}

void USER_tests() {
    USER_test_assert();
    USER_test_string();
    USER_test_math();
}
