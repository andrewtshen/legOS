#ifndef RISCV_H
#define RISCV_H

#include <stdint.h>

// which hart (core) is this?
static inline uint64_t r_mhartid() {
    uint64_t x;
    asm volatile("csrr %0, mhartid" : "=r" (x) );
    return x;
}

// Supervisor Status Register, sstatus
#define SSTATUS_SPP (1L << 8)  // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1L << 5) // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE (1L << 4) // User Previous Interrupt Enable
#define SSTATUS_SIE (1L << 1)  // Supervisor Interrupt Enable
#define SSTATUS_UIE (1L << 0)  // User Interrupt Enable

static inline uint64_t r_sstatus() {
    uint64_t x;
    asm volatile("csrr %0, sstatus" : "=r" (x) );
    return x;
}

static inline void  w_sstatus(uint64_t x) {
    asm volatile("csrw sstatus, %0" : : "r" (x));
}

// Supervisor Interrupt Pending
static inline uint64_t r_sip() {
    uint64_t x;
    asm volatile("csrr %0, sip" : "=r" (x) );
    return x;
}

static inline void  w_sip(uint64_t x) {
    asm volatile("csrw sip, %0" : : "r" (x));
}

// Supervisor Interrupt Enable
#define SIE_SEIE (1L << 9) // external
#define SIE_STIE (1L << 5) // timer
#define SIE_SSIE (1L << 1) // software
static inline uint64_t r_sie() {
    uint64_t x;
    asm volatile("csrr %0, sie" : "=r" (x) );
    return x;
}

static inline void  w_sie(uint64_t x) {
    asm volatile("csrw sie, %0" : : "r" (x));
}

// Machine-mode Interrupt Enable
#define MIE_MEIE (1L << 11) // external
#define MIE_MTIE (1L << 7)  // timer
#define MIE_MSIE (1L << 3)  // software
static inline uint64_t r_mie() {
    uint64_t x;
    asm volatile("csrr %0, mie" : "=r" (x) );
    return x;
}

static inline void  w_mie(uint64_t x) {
    asm volatile("csrw mie, %0" : : "r" (x));
}

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
static inline void  w_sepc(uint64_t x) {
    asm volatile("csrw sepc, %0" : : "r" (x));
}

static inline uint64_t r_sepc() {
    uint64_t x;
    asm volatile("csrr %0, sepc" : "=r" (x) );
    return x;
}

// Machine Exception Delegation
static inline uint64_t r_medeleg() {
    uint64_t x;
    asm volatile("csrr %0, medeleg" : "=r" (x) );
    return x;
}

static inline void  w_medeleg(uint64_t x) {
    asm volatile("csrw medeleg, %0" : : "r" (x));
}

// Machine Interrupt Delegation
static inline uint64_t r_mideleg() {
    uint64_t x;
    asm volatile("csrr %0, mideleg" : "=r" (x) );
    return x;
}

static inline void  w_mideleg(uint64_t x) {
    asm volatile("csrw mideleg, %0" : : "r" (x));
}

// Supervisor Trap-Vector Base Address
// low two bits are mode.
static inline void  w_stvec(uint64_t x) {
    asm volatile("csrw stvec, %0" : : "r" (x));
}

static inline uint64_t r_stvec() {
    uint64_t x;
    asm volatile("csrr %0, stvec" : "=r" (x) );
    return x;
}

// Machine-mode interrupt vector
static inline void  w_mtvec(uint64_t x) {
    asm volatile("csrw mtvec, %0" : : "r" (x));
}

// Supervisor Scratch register, for early trap handler in trampoline.S.
static inline void  w_sscratch(uint64_t x) {
    asm volatile("csrw sscratch, %0" : : "r" (x));
}

static inline void  w_mscratch(uint64_t x) {
    asm volatile("csrw mscratch, %0" : : "r" (x));
}

// Supervisor Trap Cause
static inline uint64_t r_scause() {
    uint64_t x;
    asm volatile("csrr %0, scause" : "=r" (x) );
    return x;
}

// Supervisor Trap Value
static inline uint64_t r_stval() {
    uint64_t x;
    asm volatile("csrr %0, stval" : "=r" (x) );
    return x;
}

// enable device interrupts
static inline void intr_on() {
    w_sstatus(r_sstatus() | SSTATUS_SIE);
}

// disable device interrupts
static inline void intr_off() {
    w_sstatus(r_sstatus() & ~SSTATUS_SIE);
}

// are device interrupts enabled?
static inline int intr_get() {
    uint64_t x = r_sstatus();
    return (x & SSTATUS_SIE) != 0;
}

static inline uint64_t r_sp() {
    uint64_t x;
    asm volatile("mv %0, sp" : "=r" (x) );
    return x;
}

// flush the TLB.
static inline void sfence_vma() {
    // the zero, zero means flush all TLB entries.
    asm volatile("sfence.vma zero, zero");
}

#define STRCONCAT(x, y) x ## y
#define STRCONCAT3(x, y, z) x ## y ## z
#define CSR_GEN(regname) \
  static inline void STRCONCAT(w_, regname)(uint64_t x) { \
    asm volatile("csrw " #regname ", %0" : : "r" (x)); \
  } \
  static inline uint64_t STRCONCAT(r_, regname)(void) { \
    uint64_t x; \
    asm volatile("csrr %0, " #regname : "=r" (x)); \
    return x; \
  }

// machine exception program counter, holds the
// instruction address to which a return from
// exception will go.
CSR_GEN(mepc)

CSR_GEN(mstatus)
#define MSTATUS_MPP_MASK (3L << 11)
#define MSTATUS_MPP_M (3L << 11)
#define MSTATUS_MPP_U (0L << 11)

// PMP Settings
CSR_GEN(pmpcfg0)
CSR_GEN(pmpcfg2)
#define PMPCFG_MASK (0xff)
#define PMPCFG(l, a, x, w, r) (((l & 1) << 7) | ((a & 3) << 3) | ((x & 1) << 2) | ((w & 1) << 1) | ((r & 1) << 0))
#define PMPCFG_A_OFF 0
#define PMPCFG_A_TOR 1
#define PMPCFG_A_NA4 2
#define PMPCFG_A_NAPOT 3
#define PMPICFG_GEN(i, regname, offset) \
    static inline void STRCONCAT3(w_pmp, i, cfg)(uint8_t cfg) { \
        uint64_t x; \
        asm volatile("csrr %0, " #regname : "=r" (x)); \
        x &= ~(((uint64_t) PMPCFG_MASK) << offset); \
        x |= ((uint64_t) cfg) << offset; \
        asm volatile("csrw " #regname ", %0" : : "r" (x)); \
    }

PMPICFG_GEN(0, pmpcfg0, 0) // -> static inline void w_pmp0cfg(uint8_t cfg) {...}
PMPICFG_GEN(1, pmpcfg0, 8)
PMPICFG_GEN(2, pmpcfg0, 16)
PMPICFG_GEN(3, pmpcfg0, 24)
PMPICFG_GEN(4, pmpcfg0, 32)
PMPICFG_GEN(5, pmpcfg0, 40)
PMPICFG_GEN(6, pmpcfg0, 48)
PMPICFG_GEN(7, pmpcfg0, 56)
PMPICFG_GEN(8, pmpcfg2, 0)
PMPICFG_GEN(9, pmpcfg2, 8)
PMPICFG_GEN(10, pmpcfg2, 16)
PMPICFG_GEN(11, pmpcfg2, 24)
PMPICFG_GEN(12, pmpcfg2, 32)
PMPICFG_GEN(13, pmpcfg2, 40)
PMPICFG_GEN(14, pmpcfg2, 48)
PMPICFG_GEN(15, pmpcfg2, 56)

CSR_GEN(pmpaddr0)
CSR_GEN(pmpaddr1)
CSR_GEN(pmpaddr2)
CSR_GEN(pmpaddr3)
CSR_GEN(pmpaddr4)
CSR_GEN(pmpaddr5)
CSR_GEN(pmpaddr6)
CSR_GEN(pmpaddr7)
CSR_GEN(pmpaddr8)
CSR_GEN(pmpaddr9)
CSR_GEN(pmpaddr10)
CSR_GEN(pmpaddr11)
CSR_GEN(pmpaddr12)
CSR_GEN(pmpaddr13)
CSR_GEN(pmpaddr14)
CSR_GEN(pmpaddr15)
#define PMPADDR_ENCODE_NAPOT(addr, region_size) (addr | ((region_size >> 3) - 1))

static inline void mret(void) {
  asm volatile("mret");
}

#endif