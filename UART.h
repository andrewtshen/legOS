#ifndef UART_H
#define UART_H

void write(char c);
char read();
char UART_int_to_hex(unsigned int s);
void UART_write_str(const char* s);
void UART_write_int(unsigned int s);
void UART_write_hex(unsigned int s);
void UART_writeln_str(const char* s);
void UART_writeln_int(unsigned int s);
void UART_writeln_hex(unsigned int s);
void UART_read_str(char* buf, int count);
void UART_read_int(unsigned int* a);

#endif
