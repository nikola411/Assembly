.section init

boot:
    .word 0xFFFFFFFF
    jmp boot

loop:
    .word 0xDEADBEEF
    jmp loop

.section loopy

boot1:
    .word 0xEEEEAAAA
    jmp boot1

loop1:
    .word 0xEAEAEAEA
    jmp loop1

halt
.end