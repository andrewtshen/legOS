/* Help:
 * https://jacobmossberg.se/posts/2018/08/11/run-c-program-bare-metal-on-arm-cortex-m3.html 
 * https://embeddedfreak.wordpress.com/2009/08/07/cortex-m3-interrupt-vector-table/
 * */
#define SVC_00 0x00
#define SVC_01 0x01

#include "UART.h"
#include "string.h"
#include "math.h"

extern unsigned int _STACK_TOP;

extern unsigned int _BSS_START;
extern unsigned int _BSS_END;
extern unsigned int _DATA_ROM_START;
extern unsigned int _DATA_RAM_START;
extern unsigned int _DATA_RAM_END;

void startup();
void main();

/* Declare our handlers */
void Reset_Handler(void);     /* Reset Handler */
void NMI_Handler(void);       /* NMI Handler */
void HardFault_Handler(void); /* Hard Fault Handler */
void MemManage_Handler(void); /* MPU Fault Handler */
void BusFault_Handler(void);  /* Bus Fault Handler */
void UsageFault_Handler(void);/* Usage Fault Handler */
void SVC_Handler(void);       /* SVCall Handler */
void DebugMon_Handler(void);  /* Debug Monitor Handler */
void PendSV_Handler(void);    /* PendSV Handler */
void SysTick_Handler(void);   /* SysTick Handler */

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
    UART_printf("In NMI_Handler!\n");
    while(1);
}
void HardFault_Handler(void) {
    UART_printf("In HardFault_Handler!\n");
    while(1);
}
void MemManage_Handler(void) {
    UART_printf("In MemManage_Handler!\n");
    while(1);
}
void BusFault_Handler(void) {
    UART_printf("In BusFault_Handler!\n");
        while(1);
}
void UsageFault_Handler(void) {
    UART_printf("In UsageFault_Handler!\n");
    while(1);
}
void DebugMon_Handler(void) {
    UART_printf("In DebugMon_Handler!\n");
    while(1);
}
void PendSV_Handler(void) {
    UART_printf("In PendSV_Handler!\n");
    while(1);
}
void SysTick_Handler(void) {
    UART_printf("In SysTick_Handler!\n");
        while(1);
}
void SVCHandler_main(unsigned int * svc_args) {
    unsigned int svc_number;    /*    * Stack contains:    * r0, r1, r2, r3, r12, r14, the return address and xPSR    * First argument (r0) is svc_args[0]    */    
    svc_number = ((char *)svc_args[6])[-2];

    switch(svc_number) {

        /* Handle SVC 00: Add from r0 and r1, display output */ 
        case SVC_00: ;
            /* Validate that it is from User Prog region */
            char *s = (char*)svc_args[0];
            int size = svc_args[1];
            if (hex_to_int("20004000") > (int)s || (int)s+size > hex_to_int("2000C000")) {
                UART_printf("ERR: Pointer given to SVC not in User Prog.\n");
                break;
            }
            UART_write_buf(s, size);
            break;

        /* Handle SVC 01: Read in a character */
        case SVC_01: ;
            /* TODO; Validate char *c */
            char *c = (char*)svc_args[0];
            *c = read();
            break;

        /* Unknown SVC */
        default:
            break;
    }
}
void Reset_Handler(void) {
    /* On reset, call startup and main */
    startup();
    main();

    /* If main() ever exit, this should hold MCU from running wild */
    while(1);
}

void startup() {
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
        *data_ram_start_p++ = *data_rom_start_p++;
    }
}