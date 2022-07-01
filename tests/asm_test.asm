.section .rodata
var:
    .word 20
x:
    .word 40
.section .data
mem:
    .word 10
.section .text
    ldr r1, 10
    ldr r2, 20
    cmp r1, r2
    jne jumpLabel
    sub r1, r2
    add r2, r4
    push r0
    pop r1
    iret
.end

