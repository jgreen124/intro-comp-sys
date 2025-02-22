# I. Foundational Concepts

- **Von Neumann Model:** Both program instructions (text) and data reside in memory.
- **Execution Cycle:**
  - Fetch Instruction
  - Decode Instruction
  - Execute Instruction
- **Memory Organization:** Key areas include Code, Globals, Stack, and Heap.
- **Run Time Storage:**
  - **Global (static) variables:** Allocated in the globals region at compile-time.
  - **Method local variables and parameters:** Allocated dynamically (run-time) on the stack.
  - **Dynamically created objects:** Allocated from the heap at run-time and garbage collected when no longer "live."
- **Program Counter (PC):** A special register that holds the pointer to the next instruction to be executed.

---

# II. Processes

- **Definition:** A process is a system abstraction representing the resources needed to execute a program. It includes the memory image and the content of registers.
- **Process Components:** Memory image, registers' content, and I/O state (files, pipes, sockets). The stack and registers' content represent the execution context or thread of control.
- **Process Memory Image:** Contains the OS, code, global variables, stack, and heap. OS data space stores file descriptors, I/O device status, etc.
- **Process Representation in Linux:** Represented by the `task_struct` C structure. Key fields include:
  - `pid_t pid`: Process identifier
  - `long state`: State of the process
  - `struct task_struct *parent`: This process’s parent
  - `struct mm_struct *mm`: Address space of this process
- **Process States:** `new`, `running`, `waiting`, `ready`, `terminated`

---

# III. Process Scheduling

- **Objectives:** Maximize CPU use by quickly switching processes onto the CPU for time sharing.
- **Process Scheduler:** Selects the next process to execute on the CPU.
- **Scheduling Queues:**
  - **Job queue:** All processes in the system.
  - **Ready queue:** Processes in main memory, ready and waiting to execute.
  - **Device queues:** Processes waiting for an I/O device.
- **Schedulers:**
  - **Short-term scheduler (CPU scheduler):** Selects which process should be executed next. Invoked frequently (milliseconds).
  - **Long-term scheduler (job scheduler):** Selects which processes should be brought into the ready queue. Invoked infrequently (seconds, minutes). Controls the degree of multiprogramming.
- **Process Types:**
  - **I/O-bound process:** Spends more time doing I/O than computations, many short CPU bursts.
  - **CPU-bound process:** Spends more time doing computations; fewer, very long CPU bursts.

---

# IV. Process Control Block (PCB)

- **Definition:** Per-process state maintained by the OS. Stored in the OS data space.
- **Contents:**
  - Identification (process, parent process, user, group, etc.)
  - Execution contexts (registers, threads)
  - Address space (virtual memory)
  - I/O state (file handlers, communication endpoints)
  - Accounting information
  - Process state (running, waiting, etc.)
  - Program counter (location of next instruction)
  - CPU registers
  - CPU scheduling information (priorities)
  - Memory-management information
  - I/O status information (I/O devices allocated, list of open files)
- **Protection:** PCB is kept in a protected memory area to prevent unauthorized user access.

---

# V. Context Switch

- **Definition:** When the CPU switches to another process, the system saves the state of the old process in its PCB and loads the saved state for the new process from its PCB.
- **Overhead:** Context-switch time is overhead, as the system does no useful work during the switch. The more complex the OS and PCB, the longer the context switch.

---

# VI. Process Creation

- **Method:** In UNIX, a process creates another process using the `fork()` system call.
- **Parent and Child:** The creating process is the parent, and the new process is the child.
- **Duplication:** The child process is created as a copy of the parent process (process image and process control structure), except for identification and scheduling state.
- **Address Spaces:** Parent and child processes run in two different address spaces with no memory sharing by default.
- **`exec()` call:** Used by the newly created process to run a different program than the parent.
- **Key Points on `fork()`:**
  - The child and parent run in separate memory spaces.
  - At the time of `fork()`, both memory spaces have the same content.
  - The child inherits a copy of memory as it is modified at the time of `fork()` by the parent.
  - Memory writes and file mappings performed by one process do not affect the other.
- **Child Process Differences:**
  - The child has its own unique process ID.
  - The child's parent process ID is the same as the parent's process ID.
  - The child does not inherit its parent's memory locks.
  - Process resource utilizations and CPU time counters are reset to zero in the child.
  - The child's set of pending signals is initially empty.
- **`execve()`:** Executes a new program, replacing the current program with newly initialized stack, heap, and data segments.
- **Process Hierarchy:** Processes can be organized hierarchically into a process tree.

---

# VII. Process Termination

- **Voluntary Termination:** Process executes its last statement and uses the `exit()` system call.
- **Involuntary Termination:**
  - Signal sent (`kill()`) from another process.
  - Termination by the OS due to an illegal operation.
- **Parent Notification:** The parent is informed via `wait()`.
- **`wait()` system call:** A process can wait for another process to finish.
- **`kill()` system call:** Can kill another process.
- **Zombie Processes:** When a child terminates before the parent calls `wait()`, the kernel retains information about it.
- **Orphan Processes:** If the parent terminates without invoking `wait()`, the child process becomes a child of the `init` process (PID = 1).
- **Cascading Termination:** If a parent process terminates, all its children are also terminated.
- **Details on `wait()`:**
  - Suspends execution of the calling thread until status information for one of its terminated child processes is available.
  - `waitpid()` is equivalent to `wait()` if the `pid` argument is `-1` and the `options` argument is `0`.
- **Macros for Interpreting Status:** `WIFEXITED()`, `WEXITSTATUS()`, `WIFSIGNALED()`, `WTERMSIG()`.
- **`waitpid()`:** A more flexible `wait()` system call that awaits a change of state of a specific or any PID process child.
- **Flags for `waitpid()`:** `WNOHANG`, `WUNTRACED`, `WCONTINUED`.
- **Status Information Macros:** `WIFEXITED(status)`, `WEXITSTATUS(status)`, `WIFSIGNALED(status)`, `WTERMSIG(status)`, `WIFSTOPPED(status)`, `WSTOPSIG(status)`, `WIFCONTINUED(status)`.
- **Zombie processes can be killed by sending the `SIGCHLD` signal to the parent, using the `kill` command.**
- **Reaping the zombie process:** Once this is done using the `wait` system call, the zombie process is eliminated from the process table.

---

# VIII. Examples and System Calls

- `fork()`: Creates a new process.
- `exec()/execve()`: Replaces the process' memory space with a new program.
- `wait()/waitpid()`: Used by a parent to wait for a child process to terminate and collect its termination status.
- `kill()`: Sends a signal to a process, often used to terminate it.
- `exit()`: Terminates the current process.
- `execlp()`: Executes a specified file with given arguments.
