#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Wrong argument!"
elif [ $1 = "asm" ]
then
    ./asembler -o out.o ./tests/asm_test0.asm
else [ $1 = "lnk" ]
    ./linker -o output.o -hex linker_test0.o linker_test1.o
fi