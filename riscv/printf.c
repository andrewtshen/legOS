#include <stdarg.h>
#include "printf.h"
#include "string.h"
#include "UART.h"
#include "math.h"

static char digits[] = "0123456789abcdef";

static void printint(int xx, int base, int sign) {
    char buf[16];
    int i;
    int x;

    if(sign && (sign = xx < 0))
        x = -xx;
    else
        x = xx;

    i = 0;
    do {
        buf[i++] = digits[x % base];
    } while((x /= base) != 0);

    if(sign)
        buf[i++] = '-';

    while(--i >= 0)
        write(buf[i]);
}

static void printptr(int x) {
    int i;
    write('0');
    write('x');
    for (i = 0; i < (sizeof(int) * 2); i++, x <<= 4)
        write(digits[x >> (sizeof(int) * 8 - 4)]);
}

/* printf formats and prints a string */
/* TODO: Replace with svc_print, merge together into one string */
void printf(char *fmt, ...) {
    va_list ap;
    int i, c;
    char *s;

    va_start(ap, fmt);
    for(i = 0; (c = fmt[i] & 0xff) != 0; i++) {
        if(c != '%') {
            write(c);
            continue;
        }
        c = fmt[++i] & 0xff;
        if(c == 0)
            break;
        switch(c) {
            case 'd':
                printint(va_arg(ap, int), 10, 1);
                break;
            case 'x':
                printint(va_arg(ap, int), 16, 1);
                break;
            case 'p':
                printptr(va_arg(ap, int));
                break;
            case 's':
                if((s = va_arg(ap, char*)) == 0)
                    s = "(null)";
                for(; *s; s++)
                    write(*s);
                break;
            case '%':
                write('%');
                break;
            default:
                // Print unknown % sequence to draw attention.
                write('%');
                write(c);
                break;
        }
    }
}

/* Use an svc call to print a char* */
void svc_print(char *c, int size) {
    /*asm volatile ("MOV R0, %0": : "r" (c));*/
    /*asm volatile ("MOV R1, %0": : "r" (size));*/
    /*asm volatile ("PUSH {LR}");*/
    /*asm volatile ("svc 0");*/
    /*asm volatile ("POP {LR}");*/
}

/* Add an int value to the print */
void add_int(unsigned int s, char *f) {
    if (s < 10) {
        strcatc(f, s + '0', 100);
    } else {
        add_int(s / 10, f);
        add_int(s % 10, f);
    }
}

/* Add a hex value to the print */
void add_hex(unsigned int s, char *f) {
    if (s < 16) {
        strcatc(f, digit_to_hex(s), 100);
    } else {
        add_hex(s / 16, f);
        add_hex(s % 16, f);
    }
}

