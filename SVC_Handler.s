.syntax unified

.global SVC_Handler
.type SVC_Handler, %function
SVC_Handler:
    TST LR, #4
    ITE EQ
    MRSEQ R0, MSP
    MRSNE R0, PSP
    PUSH {LR}
    BL SVCHandler_main
    POP {LR}
    BX LR
