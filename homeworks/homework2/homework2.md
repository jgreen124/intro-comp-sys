# Intro to Computer Systems

## Problem 1 - Forking Processes
### Suppose we write the following code below -
```c
int main(int argc, char *argv[]) {
int p = fork();
    for (int i = 0; i<=p; i++) {
        waitpid(p); // CAREFUL: this instruction is pseudo-instruction!!!
        printf ("GrPr: %d, Pr: %d, num: %d, iter = %d\n", getppid(), getpid(), p, i);
        p = fork();
    }
}
```
### 1. What is the anticipated printout? Sketch how the code works as you describe the printouts.
Here is a step by step walkthrough of the code execution:
1. The parent process runs the main program. The parent process calls the fork and creates a child process. Let's call the parent process `p1` and the child process `c1`. Let's also say that `p1` has `PID = 1000` and `c1` has `PID = 1001`.
2. `p1` enters the loop and waits at the `waitpid(p)` until the `c1` process finishes. Process `c1` enters the loop, prints out `GrPR: 1000, Pr: 1001, num: 0, iter = 0`. We see this printed statement because `p = 0` when inside the child process, and this is the first time `c1` goes through the for loop.
3. After printing the statement, process `c1` calls `fork()` and creates another child process, `c2`, with `PID = 1002`.
4. When inside process `c2`, we exit the loop and terminate the process. This is because `p = 0` when inside process `c2`, and `i = 1` after exiting the loop, so `i>p` and the loop exits. When inside process `c1`, `p = 1002` since we just created another child. Even though `i = 1`, `i <= p`, so `c1` continues.
5. Because of this, loop `c1` will continue indefinitely because it will always call `fork()`, increasing `p`. Therefore, `i>p` is impossible in process `c1`. Each time, loop `c1` will print out `GrPR: 1000, Pr: x, num: 0, iter = 0`, where x is the process id of the new child that ends up just exiting immediately.
### 2. Does this program end or does it run indefinitely?
- This program will run indefinitely because the first child process will always create new child processes and therefore never reach the end condition of the loop.
### 3. Is this a fork bomb (the number of forked processes keeps growing during execution and as a result system resources are depleted)?
- This isn't a fork bomb because each child process that is created by the first child process will end, meaning that we do not see the number of processes increasing as the program runs. The number of processes will be consistent.
### 4. How many processes are forked? How many processes run concurrently at any time?
- At most there are 3 processes forked at a time, and of those three, two will run concurrently at any time. Since the parent process will wait at the `waitpid(p)` command, it has to wait for the child process to end, which it never will. However, the child process of the child process will run at the same time as its parent because the parent doesn't have to wait for the child process to end.

# Insert Picture for Question 1 Here


## Problem 

