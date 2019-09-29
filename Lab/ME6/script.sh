#!/bin/bash

cd ~/Desktop/paulino/GitClones/CoE135/ME6	

sudo dmesg -c 

echo "---------- I. [COMPILING CPUINFO MODULE] ----------"
make
sudo insmod lab6_cpuinfo_salmon.ko
sudo rmmod lab6_cpuinfo_salmon.ko
make clean
dmesg

sed -i -e 's/lab6_cpuinfo_salmon.o/lab6_linkedlist_salmon.o/g' Makefile
sed -i -e 's/CFLAGS_cpuinfo.o/CFLAGS_linkedlist.o/g' Makefile
echo "---------- II. [COMPILING LINKED LIST MODULE] ----------"
make
sudo insmod lab6_linkedlist_salmon.ko
sudo rmmod lab6_linkedlist_salmon.ko
make clean
dmesg

sed -i -e 's/lab6_linkedlist_salmon.o/lab6_process_salmon.o/g' Makefile
sed -i -e 's/CFLAGS_linkedlist.o/CFLAGS_process.o/g' Makefile
echo "---------- III. [COMPILING PROCESS MODULE] ----------"
make
sudo insmod lab6_process_salmon.ko
sudo rmmod lab6_process_salmon.ko
make clean
dmesg

echo "Reverting Makefile to lab6_cpuinfo_salmon.o..."
sed -i -e 's/lab6_process_salmon.o/lab6_cpuinfo_salmon.o/g' Makefile
sed -i -e 's/CFLAGS_process.o/CFLAGS_cpuinfo.o/g' Makefile
echo "Script has finished executing."