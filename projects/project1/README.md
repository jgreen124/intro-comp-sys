# Project 1 IPC - Instructions

## Project Files

- **`README.md`**: This file.
- **`main.c`**: The main file to run the project.
- **`utils.h`** and **`utils.c`**: Contain utility functions for file generation and file reading.
- **`process_tree.h`** and **`process_tree.c`**: Contain functions for process trees and searches.

## How to Run

Two bash scripts are provided for convenience:

- **`compile.sh`**: Compiles the project and creates an executable file named `ipc` by default.
- **`run.sh`**: Runs the project executable.

### Default Behavior

- **`compile.sh`**:
    ```bash
    gcc main.c utils.c process_tree.c -o ipc
    ```
    Compiles the project.

- **`run.sh`**:
    ```bash
    ./ipc 20000 80 100 2
    ```
    Runs the project executable with default arguments.

Both scripts can be modified to change the compilation name and execution command as needed. The `run.sh` script also accepts command-line arguments for flexibility.

### Manual Compilation and Execution

If you are using `gcc`, compile the project with:

```bash
gcc main.c utils.c process_tree.c -o ipc
```

Run the project using the following format:

```bash
Usage: ./ipc <L> <H> <PN> <F>
        L  = Total number of integers (must be >= 20000)
        H  = Number of hidden keys (range: 40 to 80)
        PN = Number of processes (maximum depends on system limits)
        F  = Number of children per process (must be 2 or 4)
```


### Program Output
The program generates an `input.txt` and `output.txt` file in the current directory. The `input.txt` file contains the generated integers, and the `output.txt` file contains the results of the search for hidden keys. The program also prints the number of processes created and the time taken to execute the search, and status messages to the terminal.

