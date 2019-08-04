#include "math.h"
#include "UART.h"

char digit_to_hex(int x) {
    /* Conversion to Hex with switch case */
    if (x < 10) {
        return x + '0';
    } else {
        return x - 10 + 'A';
    }
}

int hex_to_decimal(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    else if (c >= 'a' && c <='f') return c - 'a' + 10;
    else if (c >= 'A' && c <='F') return  c - 'A' + 10;
    return -1;
}

int hex_to_int(char* s) {
    int val = 0;
    while (*s) {
        int c = *s++; 

        /* Convert to Hex Digit to Decimal Value */
        c = hex_to_decimal((char)c);

        /* Insert Binary Representation in Place */
        val = (val << 4) | (c & 0xF);
    }
    return val;
}

char* int_to_hex(int x, char *s) {
    for (int i = 7; i >= 0; i--) {
        int d = (x & 0xF);
        s[i] = digit_to_hex(d);
        x = x >> 4;
    }

    return s;
}