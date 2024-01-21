.section Placeholder
.word 0xFFFFFF
.section init
.skip 4
.word promenljiva
.skip 4
promenljiva:
    .word 10
.section sabiranje
    ld $10, %r1
    add %r1, %r1
    add %r1, %r1
    ld $2, %r2
    add %r2, %r1
    ld promenljiva, %r3
.end
