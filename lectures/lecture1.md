# I. Operating System (OS) Basics

- **Definition:** An OS is a resource allocator, managing all resources and deciding between conflicting requests to ensure efficient and fair resource use. It also serves as a control program, overseeing program execution to prevent errors and improper computer use.
- **Core Functions:**
  - Acts as an intermediary between a user and the computer hardware.
  - Manages hardware resources.
  - Provides a virtual machine interface that is easy and safe.
  - Offers a set of utilities to simplify application development and execution.
- **Goals:**
  - Execute user programs, making problem-solving easier.
  - Provide a convenient computer system.
  - Utilize computer hardware efficiently.
- **OS as Mechanism and Policy:** Mechanisms determine how something is done, while policies decide what will be done. Keeping them separate allows for flexibility.
- **System Calls:** Interface provided to access OS services.

## II. Computer System Structure

- **Four Components:**
  - **Hardware:** Provides basic computing resources like CPU, memory, and I/O devices.
  - **Operating System:** Controls and coordinates hardware use among applications and users.
  - **Application Programs:** Utilize system resources to solve user computing problems.
  - **Users:** People, machines, or other computers.
- **Kernel:** The "one program running at all times on the computer."
- **System Programs:** Programs that ship with the operating system.
- **Application Programs:** All other programs.
- **Layers of Abstraction:** Computer systems can be viewed as a series of layers: hardware, operating system, and applications.

## III. OS Design: Major Issues

- **Key Concerns:** Programming API, resource management, security, communication, structure, concurrency, performance, reliability, persistence, accounting, distribution, and scaling.
- **Virtual Machine (VM) Abstractions:** Processes, threads, virtual memory, file systems, messaging, and I/O devices.

## IV. Storage Management

- **Hierarchy:** Storage systems are organized in a hierarchy based on speed, cost, and volatility.
- **Main Memory:** Accessed directly by the CPU, typically volatile.
- **Secondary Storage:** Non-volatile, large capacity, extension of main memory (e.g., hard disks, solid-state drives).
- **Caching:** Copying information into a faster storage system. Main memory can be a cache for secondary storage.
- **Device Drivers:** Provide a uniform interface between device controllers and the kernel.

## V. Computer Architecture Fundamentals

- **Von Neumann Architecture:** The basis of modern computers, involving CPUs and memory.
- **Fetch-Decode-Execute Cycle:** The fundamental operation loop of a computer. The CPU fetches an instruction, decodes it, executes it, and stores the result.
- **Program Counter (PC):** A special register in the CPU that holds the address of the next instruction to be executed.
- **Registers:** Special-purpose memory within the CPU, used for faster operation.
- **Memory Indirection:** Fetching an operand "through" a memory location.
- **Conditionals and Looping:** Achieved through instructions that modify the Program Counter.

## VI. Hardware Support for OS Abstraction

- **Virtualization:** Allows multiple OSs and applications to run on a single physical server by creating virtual versions of computing resources.
- **Processor Modes:** User mode and system (kernel) mode. Certain instructions are privileged and can only be executed in kernel mode.
- **Exceptions:** Occur when the CPU encounters an instruction that cannot be executed.
- **Traps:** Special instructions that force the PC to a known address and switch the mode to system mode; used for system calls.
- **Interrupts:** External events that cause the CPU to jump to a known address.
- **Interrupt Vector:** Contains the addresses of all the interrupt service routines.

## VII. Operating-System Operations

- **Interrupt-Driven:** The OS waits for interrupts, traps, or exceptions to handle events.
- **Dual-Mode Operation:** Distinguishes between user code and kernel code. The mode bit indicates whether the system is in user mode or kernel mode.

## VIII. Memory Protection

- **Simple Protection Scheme:** Reserving addresses for the OS and using a mode register to prevent user programs from accessing OS memory.

## IX. Input/Output (I/O) Structure

- **Memory-Mapped I/O:** Mapping device registers into the memory space.
- **Interrupt-Driven I/O:** Devices signal the CPU when they have data or are ready for more.
- **Direct Memory Access (DMA):** Device controllers transfer blocks of data directly to main memory without CPU intervention.

## X. Key I/O Concepts

- **Polling:** The CPU repeatedly checks the status of a device.
- **Interrupts:** Devices signal the CPU when they need attention.
- **Programmed I/O (PIO):** The CPU loads/stores all data into device registers.
- **DMA:** Devices read and write memory directly.

## XI. History of Operating Systems

- **Key Milestones:** From the introduction of UNIX to the rise of cloud computing and AI.
