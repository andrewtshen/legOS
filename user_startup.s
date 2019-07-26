.syntax unified

.section .init_text,"ax",%progbits
.global _start 
_start:
    BLX main
spin:
    B spin