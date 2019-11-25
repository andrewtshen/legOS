#include <stdarg.h>
#include "UART.h"
#include "string.h"
#include "math.h"

// low-level driver routines for 16550a UART. (code from xv6-riscv-fall19)

// qemu puts UART registers here in physical memory.
#define UART0 0x10000000L
#define UART0_IRQ 10

// the UART control registers are memory-mapped
// at address UART0. this macro returns the
// address of one of the registers.
#define Reg(reg) ((volatile unsigned char *)(UART0 + reg))

// the UART control registers.
// some have different meanings for
// read vs write.
// http://byterunner.com/16550.html
#define RHR 0 // receive holding register (for input bytes)
#define THR 0 // transmit holding register (for output bytes)
#define IER 1 // interrupt enable register
#define FCR 2 // FIFO control register
#define ISR 2 // interrupt status register
#define LCR 3 // line control register
#define LSR 5 // line status register

#define ReadReg(reg) (*(Reg(reg)))
#define WriteReg(reg, v) (*(Reg(reg)) = (v))

void uartinit() {
    // disable interrupts.
    WriteReg(IER, 0x00);

    // special mode to set baud rate.
    WriteReg(LCR, 0x80);

    // LSB for baud rate of 38.4K.
    WriteReg(0, 0x03);

    // MSB for baud rate of 38.4K.
    WriteReg(1, 0x00);

    // leave set-baud mode,
    // and set word length to 8 bits, no parity.
    WriteReg(LCR, 0x03);

    // reset and enable FIFOs.
    WriteReg(FCR, 0x07);

    // enable receive interrupts.
    WriteReg(IER, 0x01);
}

// write one output character to the UART.
void write(char c) {
    // wait for Transmit Holding Empty to be set in LSR.
    while((ReadReg(LSR) & (1 << 5)) == 0)
        ;
    WriteReg(THR, c);
}

// read one input character from the UART.
// return -1 if none is waiting.
char read() {
    if(ReadReg(LSR) & 0x01){
        // input data is ready.
        return ReadReg(RHR);
    } else {
        return -1;
    }
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

void UART_printf(char *fmt, ...) {
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
