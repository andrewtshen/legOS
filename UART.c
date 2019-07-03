/* source: https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/ */

volatile unsigned int * const UART0DR = (unsigned int *)0x4000C000;
volatile unsigned int * const UART0FR = (unsigned int *)0x4000C018;

char read() {
    while (!((*UART0FR >> 6) & 1)) {
        // wait while transmit buffer is empty
    }
    return *UART0DR;
}

void write(char c) {
    while ((*UART0FR >> 5) & 1) {
        // wait while recieve buffer is full
    }
    *UART0DR = c;
}

void write_str(const char* s) {
    while (*s != '\0') {
        write(*s);
        s++;
    }
}

void write_int(unsigned int s) {
    if (s < 10) {
        write(s + 48);
    } else {
        write_int(s / 10);
        write_int(s % 10);
    }
}

void read_str(char* buf, int count) {
    
    for (int i = 0; i < count; i++) {    
        buf[i] = read();
        
        // Continue until we see carriage return
        if (buf[i] == '\r') {
            buf[i] = '\0';
            return;
        }
    }
    buf[count-1] = '\0';
}

void read_int(unsigned int* a) {
    *a = 0; 
    unsigned int tmp;
    while (1) {
        tmp = read();
        // Continue until we see carriage return
        if (tmp != '\r') {
            tmp -= '0';
            *a = (10 * (*a)) + tmp;
        } else {
            break;
        }
    }
}


