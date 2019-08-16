INCLUDE "mem.ld"

SECTIONS
{
    . = 0x2000A000;         /* Start of Download Prog */
    .text : 
    {   
        KEEP(*(.init_text)) /* ASM to main function */
        *(.text)            /* Text data */
    }

    _DOWNLOAD_ROM_START = .;
    .rodata :
    {
        *(.rodata)          /* Read only data */
    }
    
    _DOWNLOAD_DATA_START = .;
    .data : 
    {
        *(.data)            /* Data */
    }
    
    _DOWNLOAD_BSS_START = .;
    .bss : 
    {
        *(.bss)             /* Zero-initialied data */
    }
}
