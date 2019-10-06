#!/bin/bash

cd ~/Desktop/paulino/GitClones/CoE135/ME7	

sudo dmesg -c 

echo "---------- I. [COMPILING KMALLOC() MODULE] ----------"
make
sudo insmod lab7_part1_salmon.ko
sudo rmmod lab7_part1_salmon.ko
make clean
dmesg

sed -i -e 's/lab7_part1_salmon.o/lab7_part2_salmon.o/g' Makefile
sed -i -e 's/CFLAGS_lab7_part1_salmon.o/CFLAGS_lab7_part2_salmon.o/g' Makefile
echo "---------- II. [COMPILING VMALLOC() MODULE] ----------"
make
sudo insmod lab7_part2_salmon.ko
sudo rmmod lab7_part2_salmon.ko
make clean
dmesg

echo "Reverting Makefile to lab7_part1_salmon.o..."
sed -i -e 's/lab7_part2_salmon.o/lab7_part1_salmon.o/g' Makefile
sed -i -e 's/CFLAGS_lab7_part2_salmon.o/CFLAGS_lab7_part1_salmon.o/g' Makefile
echo "Script has finished executing."
