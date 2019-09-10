#!/bin/bash

cd ~/Desktop/paulino/GitClones/CoE135/ME4	

# hello world
echo "~~~HELLO WORLD C PROGRAM~~~"
gcc helloworld.c -Wall
strace ./a.out 2>&1 | grep write
echo " "
read

echo "~~~HELLO WORLD PYTHON PROGRAM~~~"
strace python3 helloworld.py 2>&1 | grep write
echo " "
read

# input
echo "~~~I/O C PROGRAM~~~"
gcc input.c -Wall
strace ./a.out 2>&1 | grep -e write -e read
echo " "
read

echo "~~~I/O PYTHON PROGRAM~~~"
strace python3 input.py 2>&1 | grep -e write -e read
echo " "
read

# resizeable array
echo "~~~CALLOC C PROGRAM~~~"
gcc calloc.c 
strace ./a.out 2>&1 | grep -e mmap -e munmap
echo " "

echo "~~~CALLOC PYTHON PROGRAM~~~"
strace python3 calloc.py 2>&1 | grep -e mmap -e munmap
echo " "

queue
echo "~~~QUEUE USING ARRAY C PROGRAM~~~"
gcc queue_array.c -Wall
strace ./a.out
echo " "

echo "~~~QUEUE USING ARRAY PYTHON PROGRAM~~~"
strace python3 queue_array.py
echo " "

echo "~~~QUEUE USING LINKED LIST C PROGRAM~~~"
gcc queue_linked_list.c -Wall
strace ./a.out
echo " "

echo "~~~QUEUE USING LINKED LIST PYTHON PROGRAM~~~"
strace python3 queue_linked_list.py
echo " "

file
echo "~~~FILE HANDLING C PROGRAM~~~"
gcc file.c -Wall
strace ./a.out 2>&1 | grep -e open -e write -e read -e close
echo " "

echo "~~~FILE HANDLING PYTHON PROGRAM~~~"
strace python3 file.py 2>&1 | grep -e open -e write -e read -e close
echo " "

# fork and exec
echo "~~~FORK AND EXEC C PROGRAM~~~"
gcc fork_and_exec.c -Wall
strace ./a.out 2>&1 | grep -e clone -e execve
echo " "

echo "~~~FORK AND EXEC PYTHON PROGRAM~~~"
strace python3 fork_and_exec.py 2>&1 | grep -e clone -e execve
echo " "