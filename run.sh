#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Wrong argument!"
elif [ $1 = "asm" ]
then
    ./asembler -o out.o ./tests/asm_test0.asm
elif [ $1 = "lnk" ]
then
    ./linker -o output.hex -hex ./tests/lnk_tests/linker_test0.o ./tests/lnk_tests/linker_test1.o
elif [ $1 = "emu" ]
then
    ./emulator ./output.hex
else
    echo "Wrong type of argument!"
fi