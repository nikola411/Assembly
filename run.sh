#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Wrong argument!"
elif [ $1 = "asm" ]
then
    ./asembler -o out.txt ./tests/asm_test1.txt
else [ $1 = "lnk" ]
    ./linker
fi