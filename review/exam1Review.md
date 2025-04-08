# Midterm Exam Review
- Looks over homework 2, homework 3 problem 2, lecture slide examples

## Homework 2

### Problem 1 - Forking Processes

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
int p = fork();
for (int i = 0; i<=p; i++) {
    waitpid(-1, NULL, 0); 
    printf ("GrPr: %d, Pr: %d, num: %d, iter = %d\n", getppid(), getpid(), p, i);
    p = fork();
    }
}
```

- This code will continuously print out. The first child will continuously create childs until one of the childs of the child has a PID that is lower than the number of iterations of the loop. Any child of the child will immediately end. The parent waits for the child to ends, and will iterate through until it creates a child with PID lower than the number of iterations. 
- The program does not run indefinitely. It will eventually terminate since the PIDs will eventually wrap around to a number lower than the number of iterations of the parent process.
- This is not a fork bomb since the childs of the child will always end without forking more processes and will always end before the first child forks another process. At most, there are three processes, a parent, a child, and a child of the child.

