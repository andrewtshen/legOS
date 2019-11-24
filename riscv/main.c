#include <stdarg.h>
#include <stdint.h>
#include "UART.h"
#include "printf.h"
#include "scanf.h"
#include "riscv.h"
#include "trap.h"

void main() {
    // // set M Previous Privilege mode to Supervisor, for mret.
    unsigned long x = r_mstatus();
    x &= ~MSTATUS_MPP_MASK;
    x |= MSTATUS_MPP_S;
    w_mstatus(x);

    // set M Exception Program Counter to main, for mret.
    // requires gcc -mcmodel=medany
    // w_mepc((uint64_t)main);

    // // delegate all interrupts and exceptions to supervisor mode.
    w_medeleg(0xffff);
    w_mideleg(0xffff);
    printf("here a!");
    // w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);
    printf("here b!");

    // // set up kernel trap vector
    trapinithart();

    // // switch to supervisor mode and jump to main().
    asm volatile("mret");

    char* name = "andrew";
    printf("Hello %s, it's good to meet you!\n", name);

    char animal[20];
    UART_printf("What did you see at the zoo?\n");
    UART_read_str(animal, 20);
    
    UART_printf("You went to the zoo and see a %s.\n", animal);
}
