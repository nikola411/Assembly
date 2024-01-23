| MMMM=0b0000 | mem32[gpr[A]+gpr[B]+D]<=gpr[C];
| MMMM=0b0010 | mem32[mem32[gpr[A]+gpr[B]+D]]<=gpr[C];
| MMMM=0b0001 | gpr[A]<=gpr[A]+D; mem32[gpr[A]]<=gpr[C];