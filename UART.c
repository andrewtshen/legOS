#include "UART.h"
#include "math.h"

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

void UART_write_buf(const char* s, int size) {
    for (int i = 0; i < size; i++) {
        write(*s);
        s++;
    }
}

void UART_write_str(const char* s) {
    while (*s != '\0') {
        write(*s);
        s++;
    }
}

void UART_write_int(unsigned int x) {
    if (x < 10) {
        write(x + '0');
    } else {
        UART_write_int(x / 10);
        UART_write_int(x % 10);
    }
}

void UART_write_hex(unsigned int x) {
    if (x < 16) {
        write(digit_to_hex(x));
    } else {
        UART_write_hex(x / 16);
        UART_write_hex(x % 16);
    }
}

void UART_writeln_str(const char* s) {
    while (*s != '\0') {
        write(*s);
        s++;
    }
    write('\n');
}

void UART_writeln_int(unsigned int x) {
    if (x < 10) {
        write(x + '0');
    } else {
        UART_write_int(x / 10);
        UART_write_int(x % 10);
    }
    write('\n');
}

void UART_writeln_hex(unsigned int x) {
    if (x < 16) {
        write(digit_to_hex(x));
    } else {
        UART_write_hex(x / 16);
        UART_write_hex(x % 16);
    }
    write('\n');
}

void UART_read_str(char* buf, int count) {
    for (int i = 0; i < count; i++) {    
        buf[i] = read();
        write(buf[i]);      // Echo the output
        
        /* Continue until we see carriage return */
        if (buf[i] == '\r') {
            buf[i] = '\0';  // Terminate string with Null character.
            write('\n');    // Nice printing.
            return;
        }
    }
    buf[count-1] = '\0';    // Terminate String with Null character.
    write('\n');            // Nice printing
}

void UART_read_int(unsigned int* a) {
    *a = 0; 
    unsigned int tmp;
    while (1) {
        tmp = read();
        write(tmp);         // Echo the output

        /* Continue until we see carriage return */
        if (tmp != '\r') {
            tmp -= '0';
            *a = (10 * (*a)) + tmp;
        } else {
            break;
        }
    }
    write('\n'); // Nice printing
}