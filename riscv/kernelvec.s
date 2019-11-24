#include "trap.h"
.section .kernelvec,"ax",%progbits

# interrupts and exceptions while in supervisor
# mode come here.
#
# push all registers, call kerneltrap(), restore, return
# .global kerneltrap
.global kernelvec
.type kernelvec, %function
.align 4
kernelvec:
        addi sp, sp, -256

        SW ra, 0(sp)
        SW sp, 8(sp)
        SW gp, 16(sp)
        SW tp, 24(sp)
        SW t0, 32(sp)
        SW t1, 40(sp)
        SW t2, 48(sp)
        SW s0, 56(sp)
        SW s1, 64(sp)
        SW a0, 72(sp)
        SW a1, 80(sp)
        SW a2, 88(sp)
        SW a3, 96(sp)
        SW a4, 104(sp)
        SW a5, 112(sp)
        SW a6, 120(sp)
        SW a7, 128(sp)
        SW s2, 136(sp)
        SW s3, 144(sp)
        SW s4, 152(sp)
        SW s5, 160(sp)
        SW s6, 168(sp)
        SW s7, 176(sp)
        SW s8, 184(sp)
        SW s9, 192(sp)
        SW s10, 200(sp)
        SW s11, 208(sp)
        SW t3, 216(sp)
        SW t4, 224(sp)
        SW t5, 232(sp)
        SW t6, 240(sp)

        call kerneltrap

        LW ra, 0(sp)
        LW sp, 8(sp)
        LW gp, 16(sp)
        LW t0, 32(sp)
        LW t1, 40(sp)
        LW t2, 48(sp)
        LW s0, 56(sp)
        LW s1, 64(sp)
        LW a0, 72(sp)
        LW a1, 80(sp)
        LW a2, 88(sp)
        LW a3, 96(sp)
        LW a4, 104(sp)
        LW a5, 112(sp)
        LW a6, 120(sp)
        LW a7, 128(sp)
        LW s2, 136(sp)
        LW s3, 144(sp)
        LW s4, 152(sp)
        LW s5, 160(sp)
        LW s6, 168(sp)
        LW s7, 176(sp)
        LW s8, 184(sp)
        LW s9, 192(sp)
        LW s10, 200(sp)
        LW s11, 208(sp)
        LW t3, 216(sp)
        LW t4, 224(sp)
        LW t5, 232(sp)
        LW t6, 240(sp)

        addi sp, sp, 256

        sret
.size kernelvec, .-kernelvec
