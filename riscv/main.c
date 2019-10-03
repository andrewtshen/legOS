#include <stdarg.h>
#include "UART.h"
#include "printf.h"
#include "scanf.h"

void main() {
    char* name = "andrew";
    printf("Hello %s, it's good to meet you!", name);

    char animal[20];
    UART_printf("What did you see at the zoo?\n");
    UART_read_str(animal, 20);
    
    UART_printf("You went to the zoo and see a %s.\n", animal);
}
