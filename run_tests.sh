#!/bin/bash
home="/home/nikola/Desktop/Assembly/Assembly"
tests="${home}/tests"
asm="${tests}/asm/"

cd $asm
asm_test=`ls`

for test in $asm_test
do
    echo $test
done

