#include "user.h"
#include "utility.h"
#include "printf.h"
#include "string.h"
#include "UART.h"

void append(char *f) {
    int a = 5;
    strcatc(f, a+'0');
}

int main() {
    // char *h = "hello";
    // char *w = " world";
    // char c = '!';
    // h = strcat(h, w);
    // h = strcatc(h, c);
    // int a = 15;
    // printf("%x\n", a);
    // printf("%d\n", a);
    
    // char f[10];
    // strncpy(f, "", 10);
    // strcatc(f, a + '0');
    // UART_write_str(f);

    // int a = 5;
    // char g[10];
    // strncpy(g, "", 10);
    // strcatc(g, a + '0');
    // UART_write_str(g);
    
    
    char g[10];
    strncpy(g, "", 10);
    append(&g);

    // strncpy(g, "", 10);
    // strcatc(g, 5 + '0');

    UART_write_str(g);


    // int a = 5;
    // char *f = "\0";
    // char *g = "\0";
    // UART_write_str("Location f: ");
    // UART_writeln_hex(f);
    // UART_write_str("Location g: ");
    // UART_write_hex(g);
    
    // printf("this is %d", 5);
}