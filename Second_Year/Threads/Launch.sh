#!/bin/bash
echo -e "-------------------------------------------"
for ((a = 1; a <= 16; a++))
do
echo -e "Number of threads: $a"
time ./Threads.out $a
echo -e "\n\n"
done
echo -e "-------------------------------------------"
