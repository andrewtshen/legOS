#ifndef UART_H 
#define UART_H

struct UART
{
    volatile unsigned int txdata;
    volatile unsigned int rxdata;
    volatile unsigned int txctrl;
    volatile unsigned int rxctrl;
    volatile unsigned int ie;
    volatile unsigned int ip;
    volatile unsigned int div;
};

#define UART ((struct UART*) 0x10013000)

void write(char c);
char read();
void UART_write_buf(const char* s, int size);
void UART_write_str(const char* s);
void UART_read_str(char* buf, int count);
void UART_read_int(unsigned int* a);
void UART_printf(char *fmt, ...);

#endif

