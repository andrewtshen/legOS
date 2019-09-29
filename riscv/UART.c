#include "UART.h"
#include "string.h"
#include "math.h"

char read() {
    int r = UART->rxdata;
    while ((r >> 31) & 1) {
        /* wait for full */
        r = UART->rxdata;
    }
    return r & 0xFF;
}

void write(char c) {
    while (((UART->txdata >> 31) & 1)) {
        /* wait while recieve buffer is full */
    }
    UART->txdata = c;
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

/* Add an int value to the print */
void UART_add_int(unsigned int s, char *f) {
    if (s < 10) {
        strcatc(f, s + '0', 100);
    } else {
        int s1 = s / 10;
        UART_add_int(s/10, f);
        UART_add_int(s % 10, f);
    }
}

/* Add a hex value to the print */
void UART_add_hex(unsigned int s, char *f) {
    if (s < 16) {
        strcatc(f, digit_to_hex(s), 100);
    } else {
        UART_add_hex(s / 16, f);
        UART_add_hex(s % 16, f);
    }
}

void UART_printf(char *fmt, ...) {
    char *s;
    int c, i, state;
    int *ap;
    ap = (int*)(void*)fmt + 1;         // Pointer to the next argument (void*)&fmt gives the address on the stack, 
    
    char final[100];                    // merge fmt into one string, no print is going to be longer than 100
    strncpy(final, "", 100, 100);       // cleans final

    state = 0;
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
                UART_add_int(*ap, final);

                ap++;
            } else if(c == 'x' || c == 'p') {
                UART_add_hex(*ap, final);
                ap++;
            } else if(c == 's') {
                /*UART_write_str((char*)0x80000fdc);*/
                s = (char*)*ap;
                ap++;
                /*UART_write_str(s);*/
                if(s == 0)
                    s = "(null)";
                while(*s != 0) {
                    UART_write_str("*");
                    write(*s);
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

    UART_write_str(final);
}
