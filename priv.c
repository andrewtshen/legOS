#include "priv.h"
#include "UART.h"

/* Help: https://www.cnblogs.com/shangdawei/archive/2013/04/30/3052597.html */

void switch_to_user() {
    asm volatile ("MOV r0, #0x1");
    asm volatile ("MSR PSP, r0");
    /*asm volatile ("MRS R0, PSP"); */ // Test if we are in PSP mode. 
    asm volatile ("MOV r0, #0x1");
    asm volatile ("MSR CONTROL, r0");
    asm volatile ("ISB");
}


void switch_to_kernel() {
    asm volatile ("svc 0");
}
