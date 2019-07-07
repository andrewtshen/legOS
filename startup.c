/* Sources:
 * https://jacobmossberg.se/posts/2018/08/11/run-c-program-bare-metal-on-arm-cortex-m3.html 
 * https://embeddedfreak.wordpress.com/2009/08/07/cortex-m3-interrupt-vector-table/
 * */

/*#define STACK_TOP 0x20005000*/

#include "UART.h"
extern void _STACK_TOP;

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
void (* const vectortable[])(void) = {
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
#pragma weak NMI_Handler        = Default_Handler /* NMI handler */
#pragma weak HardFault_Handler  = Default_Handler /* Hard Fault handler */
#pragma weak MemManage_Handler  = Default_Handler /* MPU Fault Handler */
#pragma weak BusFault_Handler   = Default_Handler /* Bus Fault Handler */
#pragma weak UsageFault_Handler = Default_Handler /* Usage Fault Handler */
#pragma weak DebugMon_Handler   = Default_Handler /* Debug Monitor Handler */
#pragma weak PendSV_Handler     = Default_Handler /* PendSV Handler */
#pragma weak SysTick_Handler    = Default_Handler /* SysTick Handler */

void Default_Handler(void) {
    while (1);
}

typedef void (*svcall_t)(void*);
 
void SVC_Handler(void) {
    unsigned int * cpuid = (unsigned int *)0xE000ED00;
    int val = *cpuid;
    write_str("Value: ");
    writeln_str("Reading CPUID in Kernel Mode (with SVC)");
    writeln_int(*cpuid);
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
