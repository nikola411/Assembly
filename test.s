.section ivt 
ivt1:
    .word labela1
    .skip 14

.section my_section
labela:
    ldr r1, $0
    int r1
    ldr r2, $0xFF
    halt

labela1:
   ldr r3, $0xFF
   iret
.end