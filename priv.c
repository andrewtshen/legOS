#include "priv.h"
#include "UART.h"
#include<stdint.h>

extern uint8_t _USER_TEXT_START[];

inline void switch_to_user() {
    asm volatile ("MRS r0, MSP");

    /* Change to process stack */
    asm volatile ("MOV r0, #0x20000000");
    asm volatile ("ADD r0, #0x5000"); // ask ???
    asm volatile ("MSR PSP, r0");
    asm volatile ("MRS r0, PSP");  // Test if we are in PSP mode. 

    /* Change control */
    asm volatile ("MOV r0, #0x1");
    asm volatile ("MSR CONTROL, r0");
    asm volatile ("ISB");
}


void switch_to_kernel() {
    asm volatile ("svc 0");
}
