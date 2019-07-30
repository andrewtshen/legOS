#include "MPU.h"
#include "UART.h"
#include "launch.h"

void MPU_setup() {
    asm volatile("DMB");

    MPU->CTRL = 0;              // Disable CTRL register
                                // ---
    MPU->RNR = 0;               // Select region 0, Disable all memory to user
    MPU->RBAR = 0x00000000;     // Base Address = 0x00000000
    MPU->RASR = 0x0100003F;     // Privileged only, TEX=0,S=0,C=0,B=0, 4GB (for good measure), Enable=1
                                // ---
    MPU->RNR = 1;               // Select region 1, Enable Trampoline
    MPU->RBAR = (int)&launch;   // Base Address = &launch
    MPU->RASR = 0x0300000B;     // All access, TEX=0,S=0,C=0,B=0, 64 Bytes, Enable=1
                                // Note: Bug with 32 Bytes Protection
                                // ---
    MPU->RNR = 2;               // Select region 2, Enable User Prog + User Stack
    MPU->RBAR = 0x20004000;     // Base Address = 0x20004000
    MPU->RASR = 0x0300001B;     // All Access, TEX=0,S=0,C=0,B=0, 16KB, Enable=1

    MPU->CTRL = 1;

    asm volatile("DSB");
    asm volatile("ISB");
}
