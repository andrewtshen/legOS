#ifndef UART_H
#define UART_H

void write_str(const char *s);
void write_int(unsigned int s);
void writeln_str(const char *s);
void writeln_int(unsigned int s);
void writeln_hex(unsigned int s);
void read_str(char* buf, int count);
void read_int(unsigned int* a);
void write(char c);
char read();

#endif
