#!/bin/bash

## This bash script can be used to run the compiled IPC project.
## Run "chmod +x run.sh" in the terminal to make this script executable.
## Run "./run.sh" in the terminal to execute this script.
## By default, this script will run with L = 20,000, N = 80, M = 100, and F = 2
## You can also pass in your own values for L, N, M, and F, in that order
## Example: ./run.sh 500000 50 200 4

set -e

PROGRAM_NAME="ipc"
L=${1:-20000}
N=${2:-80}
M=${3:-100}
F=${4:-2}

# Check if the program is compiled
if [ ! -f "$PROGRAM_NAME" ]; then
    echo "Error: $PROGRAM_NAME not found. Please compile the program first."
    exit 1
fi
# Run the program with the specified parameters
./$PROGRAM_NAME $L $N $M $F
# Check if the program ran successfully
if [ $? -eq 0 ]; then
    echo "Program executed successfully."
else
    echo "Program execution failed."
    exit 1
fi
