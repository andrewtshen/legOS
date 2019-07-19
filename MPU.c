#include "MPU.h"
#include "UART.h"

// Setup procedure for each region
// void MpuRegionSetup(unsigned int addr, unsigned int region,
//                     unsigned int size, unsigned int ap, 
//                     unsigned int MemAttrib, unsigned int srd, 
//                     unsigned int XN, unsigned int enable)
// { 
//     *MPU->RBAR = (addr & 0xFFFFFFE0) | (region & 0xF) | 0x10;
//     *MPU->RASR = ((XN & 0x1)<<28) | ((ap & 0x7)<<24) |
//     ((MemAttrib & 0x3F)<<16) | ((srd&0xFF)<<8) |
//     ((size & 0x1F)<<1)| (enable & 0x1);
//     return;
// }

void MPU_setup() {
    asm volatile("DMB");

    MPU->CTRL = 0;          // Disable CTRL register

    MPU->RNR = 0;           // Select region 0
    MPU->RBAR = 0x20008000; // Base Address = 0x00000000
    MPU->RASR = 0x0000002F; // No access, TEX=0,S=1,C=1,B=1, 16MB, Enable=1

    MPU->CTRL = 1;          // Activate CTRL register

    asm volatile("DSB");
    asm volatile("ISB");
}
