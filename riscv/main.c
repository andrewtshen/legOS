#include "UART.h"
#include "printf.h"
#include "scanf.h"

int sum(int a, int b) {
    return a+b;
}

void main() {
    int a, b;
    UART_read_int(&a);
    UART_read_int(&b);
    sum(a, b);
    /*UART_printf("Hello world!\n");*/
    UART_write_str("What is your name: ");
    char name[20];
    UART_read_str(name, 20);
    /*UART_write_str(name);*/
    UART_printf("Hello %s, it's good to meet you!", name);
}
