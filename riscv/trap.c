#include <stdint.h>
#include "riscv.h"
#include "trap.h"
#include "printf.h"

// in kernelvec.S, calls kerneltrap().
extern void kernelvec();

// number of elements in array
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))


// set up to take exceptions and traps while in the kernel.
void trapinithart(void) {
  w_stvec((uint32_t)kernelvec);
}

// interrupts and exceptions from kernel code go here via kernelvec,
// on whatever the current kernel stack is.
void kerneltrap() {
    int which_dev = 0;
    uint32_t sepc = r_sepc();
    uint32_t sstatus = r_sstatus();
    uint32_t scause = r_scause();

    // if((sstatus & SSTATUS_SPP) == 0)
    //     printf("PANIC: kerneltrap: not from supervisor mode");
    // if(intr_get() != 0)
    //     printf("PANIC kerneltrap: interrupts enabled");

    // if((which_dev = devintr()) == 0){
    //     printf("scause %p (%s)\n", scause, scause_desc(scause));
    //     printf("sepc=%p stval=%p\n", r_sepc(), r_stval());
    //     printf("PANIC: kerneltrap");
    // }

    // the yield() may have caused some traps to occur,
    // so restore trap registers for use by kernelvec.S's sepc instruction.
    w_sepc(sepc);
    w_sstatus(sstatus);
}

static const char * scause_desc(uint32_t stval) {
  static const char *intr_desc[16] = {
    [0] "user software interrupt",
    [1] "supervisor software interrupt",
    [2] "<reserved for future standard use>",
    [3] "<reserved for future standard use>",
    [4] "user timer interrupt",
    [5] "supervisor timer interrupt",
    [6] "<reserved for future standard use>",
    [7] "<reserved for future standard use>",
    [8] "user external interrupt",
    [9] "supervisor external interrupt",
    [10] "<reserved for future standard use>",
    [11] "<reserved for future standard use>",
    [12] "<reserved for future standard use>",
    [13] "<reserved for future standard use>",
    [14] "<reserved for future standard use>",
    [15] "<reserved for future standard use>",
  };
  static const char *nointr_desc[16] = {
    [0] "instruction address misaligned",
    [1] "instruction access fault",
    [2] "illegal instruction",
    [3] "breakpoint",
    [4] "load address misaligned",
    [5] "load access fault",
    [6] "store/AMO address misaligned",
    [7] "store/AMO access fault",
    [8] "environment call from U-mode",
    [9] "environment call from S-mode",
    [10] "<reserved for future standard use>",
    [11] "<reserved for future standard use>",
    [12] "instruction page fault",
    [13] "load page fault",
    [14] "<reserved for future standard use>",
    [15] "store/AMO page fault",
  };
  uint32_t interrupt = stval & 0x80000000L;
  uint32_t code = stval & ~0x80000000L;
  if (interrupt) {
    if (code < NELEM(intr_desc)) {
      return intr_desc[code];
    } else {
      return "<reserved for platform use>";
    }
  } else {
    if (code < NELEM(nointr_desc)) {
      return nointr_desc[code];
    } else if (code <= 23) {
      return "<reserved for future standard use>";
    } else if (code <= 31) {
      return "<reserved for custom use>";
    } else if (code <= 47) {
      return "<reserved for future standard use>";
    } else if (code <= 63) {
      return "<reserved for custom use>";
    } else {
      return "<reserved for future standard use>";
    }
  }
}