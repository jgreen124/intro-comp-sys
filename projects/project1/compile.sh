#!/bin/bash


## This bash script can be used to compile all the source files for the IPC project. 
## Run "chmod +x compile.sh" in the terminal to make this script executable.
## Run "./compile.sh" in the terminal to execute this script.



# Exit immediately if a command exits with a non-zero status
set -e

# Variables
SOURCE_FILES="main.c process_tree.c utils.c"
OUTPUT_FILE="ipc"
COMPILER="gcc"

# Compile the source files
$COMPILER $SOURCE_FILES -o $OUTPUT_FILE

# Check if the gcc command succeeded
if [ $? -eq 0 ]; then
    echo "Compilation successful. Output file: $OUTPUT_FILE"
else
    echo "Compilation failed."
    exit 1
fi