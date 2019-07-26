.syntax unified

.global launch
launch:
    LDR r0, =#0x20005000
    MSR PSP, r0
    MOV r0, #0x3
    MSR CONTROL, r0
    ISB
    LDR r0, =#0x20005001
    BX r0
    .fill 0x2, 0x2, 0x0
