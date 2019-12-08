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

    // set up pmp, protecting kernel region
    unsigned long kernel_start_pmp = 0x0;
    kernel_start_pmp |= (PMP_WD | PMP_RD | PMP_XD | PMP_TOR);
    printf("%x\n", kernel_start_pmp);
    w_pmpcfg0(kernel_start_pmp);

    unsigned long addr_start = 0x80000000;
    w_pmpaddr0(addr_start);

    unsigned long kernel_end_pmp = 0x0;
    kernel_end_pmp |= (PMP_WE | PMP_RE | PMP_XE | PMP_TOR);
    printf("%x\n", kernel_end_pmp);
    w_pmpcfg0(kernel_end_pmp);

    unsigned long addr_end = 0x90000000;
    w_pmpaddr0(addr_end);


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