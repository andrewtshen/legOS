/* source: https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/ */

volatile unsigned int * const UART0DR = (unsigned int *)0x4000C000;
volatile unsigned int * const UART0FR = (unsigned int *)0x4000C018;

char read() {
    while (!((*UART0FR >> 6) & 1)) {
        continue;
    }
    return *UART0DR;
}

/*void write(char c) {*/
    /*while (busy) {*/
        /*// wait*/
    /*}*/
    /*send_byte(c);*/
/*}*/

void print_str(const char* s) {
    while (*s != '\0') {
        *UART0DR = (unsigned int)*s;
        s++;
    }
}

void print_int(unsigned int s) {
    if (s < 10) {
        *UART0DR = s + 48;
    } else {
        print_int(s / 10);
        print_int(s % 10);
    }
}

void read_str(char a[]) {
    int curr = 0;
    while (1) {
        a[curr] = read();
        
        // Continue until we see carriage return
        if (a[curr] != '\r') {
            curr++;
        } else {
            break;
        }
    }
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


