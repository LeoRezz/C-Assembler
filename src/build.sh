#!/bin/bash

# Compile the program
gcc -g first_word_binary.c -o first_word_binary

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. The executable 'first_word_binary' has been created."
else
    echo "Compilation failed. Please check the error messages above."
fi