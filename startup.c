/* Help:
 * https://jacobmossberg.se/posts/2018/08/11/run-c-program-bare-metal-on-arm-cortex-m3.html 
 * https://embeddedfreak.wordpress.com/2009/08/07/cortex-m3-interrupt-vector-table/
 * */

#include "UART.h"
extern unsigned int _STACK_TOP;

extern unsigned int _BSS_START;
extern unsigned int _BSS_END;
extern unsigned int _DATA_ROM_START;
extern unsigned int _DATA_RAM_START;
extern unsigned int _DATA_RAM_END;

void startup();
void main();

/* Declare our handlers */
void __attribute__((weak)) Reset_Handler(void);     /* Reset Handler */
void __attribute__((weak)) NMI_Handler(void);       /* NMI Handler */
void __attribute__((weak)) HardFault_Handler(void); /* Hard Fault Handler */
void __attribute__((weak)) MemManage_Handler(void); /* MPU Fault Handler */
void __attribute__((weak)) BusFault_Handler(void);  /* Bus Fault Handler */
void __attribute__((weak)) UsageFault_Handler(void);/* Usage Fault Handler */
void __attribute__((weak)) SVC_Handler(void);       /* SVCall Handler */
void __attribute__((weak)) DebugMon_Handler(void);  /* Debug Monitor Handler */
void __attribute__((weak)) PendSV_Handler(void);    /* PendSV Handler */
void __attribute__((weak)) SysTick_Handler(void);   /* SysTick Handler */

/* Define the vector table */
__attribute__ ((section("vectors")))
void* vectortable[] = {
    &_STACK_TOP,        // The initial stack pointer
    Reset_Handler,      // The reset handler
    NMI_Handler,        // The NMI handler
    HardFault_Handler,  // The hard fault handler
    MemManage_Handler,  // The MPU fault handler
    BusFault_Handler,   // The bus fault handler
    UsageFault_Handler, // The usage fault handler
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    0,                  // Reserved
    SVC_Handler,        // SVCall handler
    DebugMon_Handler,   // Debug monitor handler
    0,                  // Reserved
    PendSV_Handler,     // The PendSV handler
    SysTick_Handler,    // The SysTick handler
};

void Default_Handler(void);
// #pragma weak NMI_Handler        = Default_Handler /* NMI handler */
// #pragma weak HardFault_Handler  = Default_Handler /* Hard Fault handler */
// #pragma weak MemManage_Handler  = Default_Handler /* MPU Fault Handler */
// #pragma weak BusFault_Handler   = Default_Handler /* Bus Fault Handler */
// #pragma weak UsageFault_Handler = Default_Handler /* Usage Fault Handler */
// #pragma weak DebugMon_Handler   = Default_Handler /* Debug Monitor Handler */
// #pragma weak PendSV_Handler     = Default_Handler /* PendSV Handler */
// #pragma weak SysTick_Handler    = Default_Handler /* SysTick Handler */

void NMI_Handler(void) {
    writeln_str("In NMI_Handler!");
    while(1);
}
void HardFault_Handler(void) {
    writeln_str("In HardFault_Handler!");
    while(1);
}
void MemManage_Handler(void) {
    writeln_str("In MemManage_Handler!");
    while(1);
}
void BusFault_Handler(void) {
    writeln_str("In BusFault_Handler!");
    while(1);
}
void UsageFault_Handler(void) {
    writeln_str("In UsageFault_Handler!");
    while(1);
}
void DebugMon_Handler(void) {
    writeln_str("In DebugMon_Handler!");
    while(1);
}
void PendSV_Handler(void) {
    writeln_str("In PendSV_Handler!");
    while(1);
}
void SysTick_Handler(void) {
    writeln_str("In SysTick_Handler!");
    while(1);
}

void Default_Handler(void) {
    writeln_str("In Default_Handler!");
    while (1);
}
 
void SVC_Handler(void) {
    // /* Testing that SVCall puts us in Kernel Mode */
    // unsigned int * cpuid = (unsigned int *)0xE000ED00;
    // int val = *cpuid;
    // write_str("Value: ");
    // writeln_str("Reading CPUID in Kernel Mode (with SVC)");
    // writeln_int(*cpuid);
    // 
    // Use SVC to put us in user mode
    asm volatile ("MOV r0, #0x1");
    asm volatile ("MSR PSP, r0");
    /*asm volatile ("MRS R0, PSP"); */ // Test if we are in PSP mode. 
    asm volatile ("MOV r0, #0x1");
    asm volatile ("MSR CONTROL, r0");
    asm volatile ("ISB");
}

void Reset_Handler(void) {
    // On reset, call startup and main
    startup();
    main();

    // If main() ever exit, this should hold MCU from running wild
    while(1);
}

void startup()
{
    /* Set memory in bss segment to zeros */
    unsigned int * bss_start_p = &_BSS_START; 
    unsigned int * bss_end_p = &_BSS_END;

    while(bss_start_p != bss_end_p)
    {
        *bss_start_p = 0;
        bss_start_p++;
    }

    /* Copy memory in data segment from its position
     * in the flash image to its position in the
     * read/write memory */
    unsigned int * data_rom_start_p = &_DATA_ROM_START;
    unsigned int * data_ram_start_p = &_DATA_RAM_START;
    unsigned int * data_ram_end_p = &_DATA_RAM_END;

    while(data_ram_start_p != data_ram_end_p)
    {
        *data_ram_start_p = *data_rom_start_p;
        data_ram_start_p++;
        data_rom_start_p++;
    }

    /* Now we are ready to start the main function */
    /*main();*/

    /*while(1) {};*/
}
