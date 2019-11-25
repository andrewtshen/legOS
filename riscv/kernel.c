#include <stdarg.h>
#include <stdint.h>
#include "UART.h"
#include "printf.h"
#include "scanf.h"
#include "riscv.h"

void user();
extern void kernelvec();

void main() {
    // set up init
    uartinit();

    // set M Previous Privilege mode to User Mode, for mret.
    unsigned long x = r_mstatus();
    x &= ~MSTATUS_MPP_MASK; // clear specific mpp bits
    x |= MSTATUS_MPP_U; // set mpp bits
    w_mstatus(x);

    // set M Exception Program Counter to user, for mret.
    // requires gcc -mcmodel=medany
    w_mepc((uint64_t)user);

    // set up kernel trap vector
    w_mtvec((uint64_t)kernelvec);

    // switch to supervisor mode and jump to main().
    asm volatile("mret");

    // char* name = "andrew";
    // printf("Hello %s, it's good to meet you!\n", name);

    // char animal[20];
    // UART_printf("What did you see at the zoo?\n");
    // UART_read_str(animal, 20);
    
    // UART_printf("You went to the zoo and see a %s.\n", animal);
}

void user() {
    printf("Hello World!\n");
    asm volatile ("ecall");
}