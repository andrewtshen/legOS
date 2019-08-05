#ifndef UART_H
#define UART_H

void write(char c);
char read();
void UART_write_buf(const char* s, int size);
void UART_write_str(const char* s);
void UART_write_int(unsigned int x);
void UART_write_hex(unsigned int x);
void UART_writeln_str(const char* s);
void UART_writeln_int(unsigned int x);
void UART_writeln_hex(unsigned int s);
void UART_read_str(char* buf, int count);
void UART_read_int(unsigned int* a);

#endif
