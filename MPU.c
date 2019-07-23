#include "MPU.h"
#include "UART.h"
/* Setup procedure for each region */
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
                            // ---
    MPU->RNR = 1;           // Select region 0
    MPU->RBAR = 0x00000000; // Base Address = 0x00000000
    MPU->RASR = 0x01000037; // Privileged only, TEX=0,S=0,C=0,B=0, 512MB, Enable=1
                            // Disable Flash
    MPU->RNR = 1;           // Select region 0
    MPU->RBAR = 0x00000000; // Base Address = 0x00000000
    MPU->RASR = 0x03000011; // Privileged only, TEX=0,S=0,C=0,B=0, 512MB, Enable=1
                            // ---
                            // Enable ALL RAM
    MPU->RNR = 2;           // Select region 1
    MPU->RBAR = 0x20000000; // Base Address = 0x20000000
    MPU->RASR = 0x03000037; // All Access, TEX=0,S=0,C=0,B=0, 512KB, Enable=1
                            // ---
    //                         // Enable User Prog
    // MPU->RNR = 2;           // Select region 2
    // MPU->RBAR = 0x20005000; // Base Address = 0x20005000, region=3, valid=1,
    // MPU->RASR = 0x0307001B; // Any access, TEX=0,S=0,C=0,B=0, 16KB, Enable=1
    //                         // Test to try to make allow user access to User prog
    // // //                         // ---
    // // //                         // Disable Peripherals and Unused
    // // MPU->RNR = 3;           // Select region 3
    // // MPU->RBAR = 0x30000014; // Base Address = 0x30000000
    // // MPU->RASR = 0x0307003F; // Privileged only, TEX=0,S=0,C=0,B=0, 4GB, Enable=1
    // //                         // Until end of memory
    // // //                         // ---
    MPU->CTRL = 1;          // Activate CTRL register

    asm volatile("DSB");
    asm volatile("ISB");
}
