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


## Problem 3: Suppose you run the following program on your shell (this is pseudo-code):
```c
#include <stdio.h>
#include <unistd.h>
int i,j = 0;
main() {
i++; // i=i+3;
printf("%d\n", i);
fork();
i+=7; // i=i+77;
for (j=0; j<27,000; j++) i+=odd(j);
printf("%d\n", i);
fork();
i+=17; // i=i+19;
printf("%d\n", i);
fork();
i+=23; // i=i+23;
printf("%d\n", i);
}
```

### 1. Please explain what you would “expect” to see on your screen at run-time of the program. You are to analyze and understand what happens with the code but not yet execute it. Use your critical thinking.
I would expect to see that when we run this program we will see the number 1 printed once, another number printed 2 times, another number printed 4 times, and another number printed 8 times. We should see 15 printouts in total:
1. The first printout is just the number 1. This is because we have one process running so it runs at the first print statement.
2. We fork the first process, now meaning we have two processes running. Both of these processes enter the for loop, repeatedly adding `odd(j)` to i, and then each of these processes print the final result after exiting the loop. These processes will print the same number.
3. Each of the processes will be forked again, meaning we now have 4 processes running now. Each of these processes add 17 to i and will print the result, meaning this number gets printed 4 times.
4. Each of these processes gets forked again, meaning we now have 8 processes running. Each of these processes add 23 to i and will print the result, meaning this number gets printed 8 times.

### 2. Is the output order deterministic (is there only one combination for the output or more)? Why yes orwhy not? List a number of potential output combinations WITHOUT EXECUTING THE PROGRAM YET. Use your critical thinking.
The output order is not deterministic because the order in which the threads finish is not guarenteed. We can basically guarentee that the first printout will be 1 because there is only one process afterwards, but after that we can't be sure of the exact order of the printouts. Below are a couple of potential orders that the processes can print:
1. process 1, process 1, process 1, process 2, process 1, processs 3, process 2, process 4, process 3, process 2, process 5, process 7, process 6, porcess 5, process 8

2. process 1, process 1, process 1, process 2, process 3, process 2, process 1, process 5,process 2, process 3, process 4, process 7, process 5, process 6, process 8 

While the order of the printouts isn't deterministic, the number of printouts is always 15, and the specific value of the numbers that get printed are deterministic. It is just that the order is not deterministic.
 