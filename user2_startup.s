.syntax unified

.section .init_text,"ax",%progbits
.global _start 
_start:
    BL main    // Branch and return from main
spin:
    B spin      // Spin after user finishes
