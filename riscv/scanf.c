#include "scanf.h"
#include "printf.h"
#include "UART.h"

char svc_read(char *c) {
    // for now use read
    return read();

    /*asm volatile ("MOV R0, %0" : : "r" (c));*/
    /*asm volatile ("PUSH {LR}");*/
    /*asm volatile ("svc 1");*/
    /*asm volatile ("POP {LR}");*/
}

void scanf(char *buf, int count) {
    for (int i = 0; i < count; i++) {
        svc_read(&buf[i]);
        printf("%c", buf[i]);      // Echo the output
        
        /* Continue until we see carriage return */
        if (buf[i] == '\r') {
            buf[i] = '\0';         // Terminate string with Null character.
            printf("%c", '\n');    // Nice printing.
            return;
        }
    }
    buf[count-1] = '\0';           // Terminate String with Null character.
    printf("%c", '\n');            // Nice printing
}

