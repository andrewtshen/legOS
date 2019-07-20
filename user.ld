SECTIONS
{
    . = 0x20005000;         /* Start of User Program */
    _USER_START = .;

    . = 0x20006000;         /* 0x20005000 - 0x20006000 is the stack */
    _STACK_TOP = .;

    _USER_TEXT_START = .;   /* Text always starts at 0x20006000 */
    .text : 
    {   
        KEEP(*(.init_text))
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
}
