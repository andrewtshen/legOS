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
                                // Enable trampoline
    MPU->RNR = 1;               // Select region 1
    MPU->RBAR = (int)&launch;   // Base Address = &launch
    MPU->RASR = 0x03000009;     // All access, TEX=0,S=0,C=0,B=0, 32 Bytes, Enable=1
                                // ---
                                // Enable User Prog + User Stack
    MPU->RNR = 2;               // Select region 2
    MPU->RBAR = 0x20004000;     // Base Address = 0x20004000
    MPU->RASR = 0x0300001B;     // All Access, TEX=0,S=0,C=0,B=0, 16KB, Enable=1

    MPU->CTRL = 1;

    asm volatile("DSB");
    asm volatile("ISB");
}
