#include "UART.h"

volatile unsigned int * const UART0DR = (unsigned int *)0x4000C000;
volatile unsigned int * const UART0FR = (unsigned int *)0x4000C018;

char read() {
    while (!((*UART0FR >> 6) & 1)) {
        /* wait while transmit buffer is empty */
    }
    return *UART0DR;
}

void write(char c) {
    while ((*UART0FR >> 5) & 1) {
        /* wait while recieve buffer is full */
    }
    *UART0DR = c;
}