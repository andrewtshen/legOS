#include "printf.h"
#include "string.h"
#include "UART.h"

void printf(char *fmt, ...) {
    char *s;
    int c, i, state;
    int *ap;
    char final[100]; // merge fmt into one string, no print is going to be longer than 80
    strncpy(final, "", 100);

    state = 0;
    ap = (int*)(void*)&fmt + 1; // Pointer to the next argument (void*)&fmt gives the address on the stack, first argument after the format string
    for(i = 0; fmt[i] != '\0'; i++) { // as long as not null byte
        c = fmt[i];
        if(state == 0) {
            if(c == '%') {
                state = '%';
            } else {
                svcprint(c);
            }
        } else if(state == '%') {
            if(c == 'd'){
                write_int(*ap, &final[0]);
                UART_write_str("Final: ");
                UART_writeln_str(final);

                ap++;
            } else if(c == 'x' || c == 'p') {
                write_hex(*ap, final);
                UART_write_str("Final: ");
                UART_writeln_str(final);
                ap++;
            } else if(c == 's') {
                s = (char*)*ap;
                ap++;
                if(s == 0)
                    s = "(null)";
                while(*s != 0) {
                    svcprint(*s);
                    s++;
                }
            } else if(c == 'c') {
                svcprint(*ap);
                ap++;
            } else if(c == '%') {
                svcprint(c);
            } else {
                // Unknown % sequence.  Print it to draw attention.
                svcprint('%');
                svcprint(c);
            }
            state = 0;
        }
    }
}

void svcprint(char c) {
    asm volatile ("MOV R0, %0": : "r" (c));
    asm volatile ("PUSH {LR}");
    asm volatile ("svc 0");
    asm volatile ("POP {LR}");
}

char int_to_hex(unsigned int s) {
    /* Conversion to Hex with switch case */
    switch (s) {
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
        default:
            return s+'0';
    }
}

void write_int(unsigned int s, char **f) {
    if (s < 10) {
        // svcprint(s + '0');
        strcatc(*f, s + '0');
        UART_write_str("f: ");
        UART_writeln_str(*f);
    } else {
        write_int(s / 10, f);
        write_int(s % 10, f);
    }
}

void write_hex(unsigned int s, char *f) {
    if (s < 16) {
        // svcprint(int_to_hex(s));
        strcatc(f, int_to_hex(s));
    } else {
        write_hex(s / 16, f);
        write_hex(s % 16, f);
    }
}