#!/bin/bash

cd ~/Desktop/paulino/GitClones/CoE135/ME4	

# hello world
echo "~~~HELLO WORLD C PROGRAM~~~"
gcc helloworld.c -Wall
./a.out
echo " "

echo "~~~HELLO WORLD PYTHON PROGRAM~~~"
python3 helloworld.py
echo " "

# input
echo "~~~I/O C PROGRAM~~~"
gcc input.c -Wall
./a.out
echo " "

echo "~~~I/O PYTHON PROGRAM~~~"
python3 input.py
echo " "

# resizeable array
echo "~~~CALLOC C PROGRAM~~~"
gcc calloc.c -Wall
./a.out
echo " "

echo "~~~CALLOC PYTHON PROGRAM~~~"
# python3 calloc.py
echo " "

# file
echo "~~~FILE HANDLING C PROGRAM~~~"
gcc file.c -Wall
./a.out
echo " "

echo "~~~FILE HANDLING PYTHON PROGRAM~~~"
python3 file.py
echo " "