#include "printf.h"
#include "string.h"
#include "UART.h"
#include "math.h"

/* printf formats and prints a string */
void printf(char *fmt, ...) {
    char *s;
    int c, i, state;
    int *ap;
    char final[100];                    // merge fmt into one string, no print is going to be longer than 100
    strncpy(final, "", 100, 100);       // cleans final

    state = 0;
    ap = (int*)(void*)&fmt + 1;         // Pointer to the next argument (void*)&fmt gives the address on the stack, 
                                        // first argument after the format string
    for(i = 0; fmt[i] != '\0'; i++) {
        c = fmt[i];
        if(state == 0) {
            if(c == '%') {
                state = '%';
            } else {
                strcatc(final, c, 100);
            }
        } else if(state == '%') {
            if(c == 'd'){
                add_int(*ap, final);

                ap++;
            } else if(c == 'x' || c == 'p') {
                add_hex(*ap, final);
                ap++;
            } else if(c == 's') {
                s = (char*)*ap;
                ap++;
                if(s == 0)
                    s = "(null)";
                while(*s != 0) {
                    strcatc(final, *s, 100);
                    s++;
                }
            } else if(c == 'c') {
                strcatc(final, *ap, 100);
                ap++;
            } else if(c == '%') {
                strcatc(final, c, 100);
            } else {
                /* Unknown % sequence.  Print it to draw attention. */
                strcatc(final, '%', 100);
                strcatc(final, c, 100);
            }
            state = 0;
        }
    }

    svc_print(final, strlen(final));
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

