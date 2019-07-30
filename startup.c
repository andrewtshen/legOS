/* Help:
 * https://jacobmossberg.se/posts/2018/08/11/run-c-program-bare-metal-on-arm-cortex-m3.html 
 * https://embeddedfreak.wordpress.com/2009/08/07/cortex-m3-interrupt-vector-table/
 * */
#define SVC_00 0x00
#define SVC_01 0x01


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

// void __svc(SVC_00) svc_zero(const char *string);
// void __svc(SVC_01) svc_one(const char *string);

// int call_system_func(void) {
//     svc_zero("String to pass to SVC handler zero");
//     svc_one("String to pass to a different OS function");
// }
//  
void SVCHandler_main(unsigned int * svc_args) {
    // writeln_str("---\nExecuting SVCall.");
    unsigned int svc_number;    /*    * Stack contains:    * r0, r1, r2, r3, r12, r14, the return address and xPSR    * First argument (r0) is svc_args[0]    */    
    svc_number = ((char *)svc_args[6])[-2];

    switch(svc_number) {
        case SVC_00: ;          /* Handle SVC 00: Add from r0 and r1, display output */ 
            int num0 = svc_args[0];
            write_str("Number Input: ");
            writeln_int(num0);

            int num1 = svc_args[1];
            write_str("Number Input: ");
            writeln_int(num1);

            int sum = num0+num1;
            write_str("Sum: ");
            writeln_hex(sum);
            break;
        case SVC_01: ;          /* Handle SVC 01 */            
            break;        
        default:     ;          /* Unknown SVC */
            break;    
    }
}

void SVC_Handler(void) {
    asm volatile ("MOV R0, 100");

    asm volatile (
        // "IMPORT SVCHandler_main\n\t"
        "TST LR, #4\n\t"
        "ITE EQ\n\t"
        "MRSEQ R0, MSP\n\t"
        "MRSNE R0, PSP\n\t"
        "B SVCHandler_main\n\t"
    );
    // SVCHandler_main();
}

void Reset_Handler(void) {
    /* On reset, call startup and main */
    startup();
    main();

    /* If main() ever exit, this should hold MCU from running wild */
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
}
