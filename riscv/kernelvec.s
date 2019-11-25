.section .kernelvec,"ax",%progbits

# interrupts and exceptions while in all modes come here.

.global kernelvec
.type kernelvec, %function
.align 4
kernelvec:
        j kernelvec
.size kernelvec, .-kernelvec
