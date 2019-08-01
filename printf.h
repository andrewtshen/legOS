#ifndef PRINTF_H
#define PRINTF_H

void foo();
void svcprint(char c);
void cprintf(char *fmt, ...);
// void write_str(const char *s);
void write_int(unsigned int s);
// void writeln_str(const char *s);
// void writeln_int(unsigned int s);
void write_hex(unsigned int s);
// void writeln_hex(unsigned int s);
// void read_str(char* buf, int count);
// void read_int(unsigned int* a);

#endif