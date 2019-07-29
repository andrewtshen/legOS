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

void write_str(const char* s) {
    while (*s != '\0') {
        write(*s);
        s++;
    }
}

void write_int(unsigned int s) {
    if (s < 10) {
        write(s + '0');
    } else {
        write_int(s / 10);
        write_int(s % 10);
    }
}

void write_hex(unsigned int s) {
    if (s < 16) {
        write(int_to_hex(s));
    } else {
        write_hex(s / 16);
        write_hex(s % 16);
    }
}

void writeln_str(const char* s) {
    while (*s != '\0') {
        write(*s);
        s++;
    }
    write('\n');
}

void writeln_int(unsigned int s) {
    if (s < 10) {
        write(s + '0');
    } else {
        write_int(s / 10);
        write_int(s % 10);
    }
    write('\n');
}

void writeln_hex(unsigned int s) {
    if (s < 16) {
        write(int_to_hex(s));
    } else {
        write_hex(s / 16);
        write_hex(s % 16);
    }
    write('\n');
}

void read_str(char* buf, int count) {  
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

void read_int(unsigned int* a) {
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