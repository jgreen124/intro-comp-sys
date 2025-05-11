# Midterm 2 Review

## Signals, `signal()` and `sigaction()` Registration Functions

### Slides 3–13

---

### General Signals Overview

- Signals allow the OS or applications to notify user programs of events.
- Signals are identified by numbers 1–31 and have no arguments.
- Signal names are self-explanatory.

#### Example:

- `SIGKILL` (signal #9) tells a program to terminate.

#### Key Characteristics:

- Signals act like interrupts.
- A process receiving a signal:
  - Stops normal execution
  - Jumps to the signal handler
- To kill a process:
  ```bash
  pkill -f -9 hw1_Pr9
  ```

---

### What Happens When an Event Occurs?

1. The kernel handles the event.
2. The kernel modifies the process's stack:
   - Mimics a function call to the signal handler.
3. The handler runs first when the process resumes.
4. The handler acts like a function inside `main()`, but:
   - It is asynchronous.
   - It can run at any point in `main()`.

#### Note:

- Executing a signal handler causes loss of kernel stack.
- At the end of the signal handler, a system call restores the original context.

---

### List of Signals

- `kill -l`: list all signals
- `man 7 signal`: more info

#### Signal Numbers on Different Systems

| Signal    | Linux | FreeBSD | MacOS | Solaris |
| --------- | ----- | ------- | ----- | ------- |
| SIGHUP    | 1     | 1       | 1     | 1       |
| SIGINT    | 2     | 2       | 2     | 2       |
| SIGQUIT   | 3     | 3       | 3     | 3       |
| SIGILL    | 4     | 4       | 4     | 4       |
| SIGTRAP   | 5     | 5       | 5     | 5       |
| SIGABRT   | 6     | 6       | 6     | 6       |
| SIGIOT    | 6     | 6       | 6     | 6       |
| SIGBUS    | 7     | 10      | 10    | 10      |
| SIGEMT    | -     | 7       | 7     | -       |
| SIGFPE    | 8     | 8       | 8     | 8       |
| SIGKILL   | 9     | 9       | 9     | 9       |
| SIGUSR1   | 10    | 30      | 16    | 16      |
| SIGSEGV   | 11    | 11      | 11    | 11      |
| SIGUSR2   | 12    | 31      | 17    | 17      |
| SIGPIPE   | 13    | 13      | 13    | 13      |
| SIGALRM   | 14    | 14      | 14    | 14      |
| SIGTERM   | 15    | 15      | 15    | 15      |
| SIGSTKFLT | 16    | -       | -     | 7       |
| SIGCHLD   | 17    | 20      | 18    | 18      |
| SIGCLD    | -     | -       | 18    | -       |
| SIGCONT   | 18    | 19      | 25    | 26      |
| SIGSTOP   | 19    | 17      | 23    | 24      |
| SIGTSTP   | 20    | 18      | 24    | 25      |
| SIGTTIN   | 21    | 21      | 26    | 27      |
| SIGTTOU   | 22    | 22      | 27    | 28      |
| SIGURG    | 23    | 16      | 21    | 29      |
| SIGXCPU   | 24    | 24      | 30    | 12      |
| SIGXFSZ   | 25    | 25      | 31    | 30      |
| SIGVTALRM | 26    | 26      | 28    | 20      |
| SIGPROF   | 27    | 27      | 29    | 21      |
| SIGWINCH  | 28    | 28      | 20    | 23      |
| SIGIO     | 29    | 23      | 22    | 22      |
| SIGPWR    | 30    | 29      | 19    | 19      |
| SIGSYS    | 31    | 12      | 12    | 31      |

---

### Signal Functions

| Signal  | Standard | Action | Comment                           |
| ------- | -------- | ------ | --------------------------------- |
| SIGABRT | P1990    | Core   | Abort from abort(3)               |
| SIGALRM | P1990    | Term   | Alarm timer                       |
| SIGBUS  | P2001    | Core   | Bus error (invalid memory access) |
| SIGCHLD | P1990    | Ign    | Child terminated or stopped       |
| SIGFPE  | P1990    | Core   | Floating-point exception          |
| SIGHUP  | P1990    | Term   | Hangup on controlling terminal    |
| SIGILL  | P1990    | Core   | Illegal instruction               |
| SIGINT  | P1990    | Term   | Interrupt from keyboard (Ctrl+C)  |
| SIGKILL | P1990    | Term   | Kill signal                       |
| SIGPIPE | P1990    | Term   | Broken pipe                       |
| SIGQUIT | P1990    | Core   | Quit from keyboard                |
| SIGSEGV | P1990    | Core   | Segmentation fault                |
| SIGSTOP | P1990    | Stop   | Stop process                      |
| SIGTERM | P1990    | Term   | Termination signal                |
| SIGTSTP | P1990    | Stop   | Terminal stop signal (Ctrl+Z)     |
| SIGUSR1 | P1990    | Term   | User-defined signal 1             |
| SIGUSR2 | P1990    | Term   | User-defined signal 2             |
| SIGCONT | P1990    | Cont   | Continue if stopped               |

---

### Sending Signals

- `kill -9 5595`: sends `SIGKILL`
- `kill -SIGINT 5595`: sends `SIGINT`
- `pkill -f SIGKILL ExecProc`: sends `SIGKILL` by name
- `kill(getpid(), SIGSTOP)`: from C code
- `Ctrl+C`: sends `SIGINT`
- `Ctrl+Z`: sends `SIGTSTP`
- `Ctrl+/`: sends `SIGQUIT`

---

### Usefulness of Signals

- Signals report abnormal or interesting events.
- Can be used for:
  - Debugging
  - Inter-process communication (IPC)
  - Triggering reloads or restarts
- Handlers can override default actions (with caution).

---

## Using `signal()`

### Syntax

```c
signal(<signal_number>, <handler_function_pointer>);
```

- `SIG_DFL`: default action
- `SIG_IGN`: ignore signal

### Declaration

```c
void (*signal(int sig, void (*func)(int)))(int);
```

---

### Example: `signal.c`

```c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sig_handler(int signum) {
  printf("\nReceived signal %d", signum);
}

int main() {
  signal(SIGINT, sig_handler);
  printf("Send a signal now\n");
  sleep(10);
  return 0;
}
```

---

### Example: `signal2.c`

```c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void signal_callback_handler(int signum) {
  printf("\nCaught signal %d\n", signum);
  exit(signum);
}

int main() {
  signal(SIGINT, signal_callback_handler);

  while (1) {
    printf("Program is in fact programming\n");
    sleep(1);
  }

  return EXIT_SUCCESS;
}
```

---

## Reinstalling Signal Handlers

Some systems reset signal handlers after they're triggered. To make the handler persistent:

### Example: `reinstallHandler.c`

```c
int main() {
  int (*oldHandler)();

  printf("Press CTRL - C and check\n");
  sleep(5);

  oldHandler = signal(SIGINT, SIG_IGN);
  printf("Am I protected?\n");
  sleep(5);

  signal(SIGINT, oldHandler);
  printf("Am I still protected?\n");
  sleep(5);

  printf("end of snippet\n");
}
```

---

## SIGCHLD

- Sent when a child:
  - Terminates normally
  - Is killed by a signal
  - Stops (e.g., via `SIGSTOP`)
- Parent process uses it to call `waitpid()` asynchronously.

### Example (loop to reap all children):

```c
while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
  // handle child
}
```

---

## Fork and Exec Signal Inheritance

- After `fork()`: signal masks and handlers are copied.
- After `exec()`: signal **handlers** are reset, but the signal **mask** persists.

---

## Slides 34–44: `sigaction()`

### Syntax

```c
int sigaction(signum, &action, NULL);
```

- `signum`: the signal number to handle
- `action`: pointer to `struct sigaction`

### `sa_mask`

- Signals to block during execution of the handler
- The triggering signal is automatically blocked
- Use `SA_NODEFER` to avoid blocking it

---

## Threads and Thread Signaling

### Slides 11-34

Threading: CPU switches between different software threads. Context switching happens fast enough that processes look to run in parallel

- In multicore systems, each core can multithread.
- In short, a processor has `x` cores, and the OS sees `2x` virtual cores due to multithreading

Software threads are separated in to user threads and kernel Threads

- User threads are handled in user space by user libraries
- Kernel threads are handled in kernel space

Possible multithreading implementations

- Simultaneous: processor runs independent instructions from multiple threads at same time (context switch triggered by OS)
- Hardware multiplexing: Context Switch ishardware based and triggered by memory latency and I/O (in other words, CPU triggers context switch)

For triggering context switches

- Hardware threads triggered by CPU
- Kernel threads triggered by OS
- User threads triggered by user library

Thread Implementation:

- Kernel Level threads: Threads the kernel knows about handled by kernel (such as scheduling), and kernel sees multiple execution contexts
- User level threads: managed by libraries, only see one execution context

#### Pthreads

Library used for threading in POSIX Systems

Example: see `sumWithThreads.c`

- The code spawns a thread to compute a summation

Pthread code for joining 10 Threads

```c
#define NUM_THREADS 10;

pthread_t workers[NUM_THREADS];

for (int i = 0; i < NUM_THREADS; i++) {
pthread_join(workers[i], NULL);
}
```

### Signal Handling

### Slides 47-54

Signals notify a process that something has happened

A signal handler processes Signals

- Signal is generated by particular event
- Signal is delivered to process
- Signal is handled by default or user defined handler

Every signal has a defualt handler that can be overriden with a user-defined handler.

For multi-threaded processes:

- Deliver signal to thread to which the signal applies (synchronous signals)
- Deliver signal to every thread in process
- Deliver signal to certain threads in process
- Assign a specific thread to receive all signals

UNIX standard function for delivering signal: `kill (pid_t pid, int signal)`

Synchronous signals need to be delivered to thread causing signal and not to other threads, while asynchronous ones need to be sent to generally a couple or all threads (like CTRL-C for example)

Generally, the first thread that isn't blocking a signal is the one that handles a signal unless the signal needs to be received by every thread. This is also normally the thread with the lowest tid

For pthread signal delivery: `Pthread_kill (pthread_t tid, int signal)`

### Practice Problems

### Slides 60-63

#### Problem 1

Multi-threaded Web-server: Each thread serves one incoming request by loading a video file from a disk using OS system calls. There are thousand incoming requests and only limited threads to serve the requests.
Question: User threads or Kernel threads should be used? Why?

- Use kernel threads because each thread will make blocking I/O calls and kernel threads won't be blocked. If user-threads are used, one thread will block all others

Multi-threaded web-server for Twitter, performing many and diverse tasks: loads ads from followers, news, checks for new messages. Twitter has a very big data storage capacity in all levels of hierarchy.Question: How would you structure your multithreaded server now (prefer to use user or
kernel threads)?

- Use user-threads because threads won't make blocking calls and user-threads are generally more efficient than kernel threads

#### Problem 2

Question: How many threads is it preferable to spawn on a multi-core processor of 4 hyper-threaded cores to execute either 256 CPU-intense or 256 I/O intense multithreaded applications:
(a) 4 for CPU intense and 4 for I/O intense
(b) 4 for CPU intense and 32 for I/O intense
(c) 8 for CPU intense and 64 for I/O intense
(d) 32 for CPU intense and 8 for I/O intense
(e) 64 for CPU intense and 128 for I/O intense
(f) 128 for CPU intense and 64 for I/O intense
(g) 256 for CPU intense and 256 for I/O intense

- C: for CPU intense tasks, use all available virtual cores (8 in this case). For I/O intense tasks, threads often idle, so having more threads allows CPU to do more things while some threads wait
- In short, you want to balance maximizing resource utilization while not causing too large of an overhead

#### Problem 3

Question: Describe a potential configuration for hardware threads – software threads
(user and kernel threads) for an environment of 4 physical cores, applying Intel’s hyper-
threading. Let’s assume that our OS is such that restricts the total number of software
threads to 100.
