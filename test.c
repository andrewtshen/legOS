#include "test.h"
#include "UART.h"
#include "priv.h"
#include "MPU.h"

volatile unsigned int * const SYSTICK = (unsigned int *)0xE000E01C;
volatile unsigned int * const CPUID = (unsigned int *)0xE000ED00;

void test_privileges() {
    writeln_str("Reading CPUID in Kernel Mode");
    write_str("Value: ");
    writeln_int(*CPUID);
    
    // Try switching to user mode 
    switch_to_user();

    writeln_str("\nReading CPUID in User Mode");
    write_str("Value: ");
    writeln_int(*CPUID);

    // Try switching to kernel mode
    writeln_str("\nSwitch to kernel mode");
    switch_to_kernel();
    writeln_str("Exiting kernel mode");
    
    writeln_str("\nReading CPUID in User Mode");
    write_str("Value: ");
    writeln_int(*CPUID);
}

void test_MPU() {
    volatile int *noaccess = (int *)0x20008004;
    *noaccess = 2;
    write_str("No access: ");
    writeln_int(*noaccess);

    writeln_str("Setting Up MPU");
    MPU_setup();
    writeln_str("Finished Setting Up MPU.");
    *noaccess = 3;
    write_str("No access: ");
    writeln_int(*noaccess);
}