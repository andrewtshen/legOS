#include <stdarg.h>
#include <stdint.h>
#include "UART.h"
#include "printf.h"
#include "scanf.h"
#include "riscv.h"

void user();
extern void kernelvec();

// Retrieve Proper NAPOT Address for Base and Size
uint64_t get_pmp_napot_addr(uint64_t base, uint64_t size) {
    uint64_t napot_size = ((size/2)-1);
    //napot_size = 0x7FFF
    uint64_t pmp_addr = (base + napot_size)>>2;
    //pmp_addr = 0x1000_01FF
    return pmp_addr;
}

static void pmp_init(void) {
    // PMP region 0: user has RWX to their memory
    w_pmpaddr0(get_pmp_napot_addr(0x80000000, 0x800000));
    w_pmp0cfg(PMPCFG(0, PMPCFG_A_NAPOT, 1, 1, 1));

    // PMP region 1: enable uart for all users
    w_pmpaddr1(get_pmp_napot_addr(0x10000000, 0x800000));
    w_pmp1cfg(PMPCFG(0, PMPCFG_A_NAPOT, 1, 1, 1));

    // PMP region 2..6: unused

    // PMP region 8: user has no access to entire memory range
    w_pmpaddr8((~0L) >> 1);
    w_pmp8cfg(PMPCFG(0, PMPCFG_A_NAPOT, 0, 0, 0));
}

void main() {
    // set up init
    uartinit();

    // set up pmp
    pmp_init();

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
    mret();
}

void user() {
    printf("Hello World!\n");
    asm volatile ("ecall");
}