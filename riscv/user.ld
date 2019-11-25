_stack_size = 0x1000;

SECTIONS
{
    . = 0x80101000;         /* 0x80100000 - 0x80101000 is the stack */
    _USER_STACK_TOP = .;
                            /* This program will use 0x80100000 - 0x80200000 */
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
    ASSERT((. < 0x80200000), "overflowed available area")
}
