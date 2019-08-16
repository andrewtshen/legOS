INCLUDE "mem.ld"

SECTIONS
{
    .  = 0x0;         /* From 0x00000000 */

    .text : 
    {
        *(vectors)    /* Vector table */
        *(.text)      /* Program code */
    }
    .rodata : 
    {
        *(.rodata)    /* Read only data */
    }
    _DATA_ROM_START = .;

    _STACK_TOP = 0x20001000;

    .  = 0x20001000;  /* Put RAM after Stack */      

    _DATA_RAM_START = .;
    .data : AT(_DATA_ROM_START) 
    {
        *(.data)      /* Data memory */
    }
    _DATA_RAM_END = .;

    _BSS_START = .;   /* Indicates where BSS section starts in RAM */
    .bss :
    {
        *(.bss)       /* Zero-filled run time allocate data memory */
    }
    _BSS_END = .;     /* Indicates where BSS section ends in RAM */

    . = 0x2000A000;
    _DOWNLOAD_START = .;
    . += 0x3000;     /* Reserve space for our download program */
    _DOWNLOAD_END = .;
}