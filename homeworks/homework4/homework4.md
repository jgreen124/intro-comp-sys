# Homework 4 Questions 2 and 5

## Question 2

```c

// for pipe
int pg[2];

// handler, will exit with code 4
void handler(int signum) {
    exit(4);
    f1(11, -1);
}

int main(void) {

// Initialize ints, char, and pid array
int i, j, n; char c[10]; pid_t pid[4]; //variable declarations

// Set SIGUSR1 signal to use handler
signal(SIGUSR1, handler);

// Create a pipe pg
pipe(pg);

// Create 5 children (each child has pid[i] == 0 and therefore will never )
for (i = 0; i < 5; i++) {
    // Fork a child
    pid[i] = fork();

    // Only a child will execute this part of the code
    if (pid[i] == 0) {

        // sleep and read from pipe
        sleep(3);
        n = read(pg[0], c, 2*i+1);

        if (i<3) { // exit with status 2-i
            exit(2-i);
        } else { // call f1 and never return
            f1(i,n);
        }
    }
}

// Kill i-2 which in this case will be pid[3]
kill(pid[i - 2], SIGUSR1);

for (i = 0; i < 5; i++) {
    wait(&n);
    write(pg[1], c, WEXITSTATUS(n));
}

f1(i, -1);
return 0;

}
```

I am going to assume that `pid_t pid` is supposed to have size 5 instead of 4, otherwise when we iterate through the for loop, we end up with memory errors.

In steady state, the process tree looks as follows:
```
  parent_process
  |_ child_process_with_pid[4]
```

Despite the parent forking 5 times, there will ultimately only be the parent and the last process left in steady state. This is because any child created within iteration i<3 will exit, and the last remaining process will run `f1()` without ever returning.

Despite the fact that each child process will block while waiting to read, the parent sends a SIGUSR1 to the process with pid = pid[3].
Because of this, this process will jump to the signal handler, meaning that it will terminate. And once this child terminates, the parent continues to write to the pipe. We cannot know the exact order in which the children will read from the pipe, but all we need to know is that one of the children will read, and exit successfully. The other children will block trying to read from the pipe if it is empty or just continue reading the data still left in the pipe. If all other children successfully read from the pipe, then they all exit successfully, except the child with pid = pid[4], which runs function f1() with no return. If the other children continue to block, they will still eventually terminate successfully, because whichever child from the pipe reads first will cause the parent to continue to the next loop iteration, writing to the pipe again. Therefore, the only two processes that will be in steady state are the parent and the final child created by the parent.


For the child process with pid = pid[4], the function that results in steady state is `f1(i,n);`
For the parent process, the function that results in steady state is `wait(&n)` because it will wait for the last child process to terminate but it never does because the child calls `f1(i,n)`
