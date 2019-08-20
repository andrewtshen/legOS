INCLUDE "mem.ld"

SECTIONS
{
    . = 0x20005000;         /* 0x20004000 - 0x20005000 is the stack */
    _USER_STACK_TOP = .;
                            /* This program will use 0x20005000 - 0x20006000 */
    .text : 
    {   
        KEEP(*(.init_text)) /* ASM to main function */
        *(.text)            /* Text data */
    }

    _USER_ROM_START = .;
    .rodata :
    {
        *(.rodata)          /* Read only data */
    }
    
    _USER_DATA_START = .;
    .data : 
    {
        *(.data)            /* Data */
    }
    
    _USER_BSS_START = .;
    .bss : 
    {
        *(.bss)             /* Zero-initialied data */
    }
    ASSERT((. < 0x20008000), "overflowed available area")
}
