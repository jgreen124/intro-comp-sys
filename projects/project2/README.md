# Multithreaded Signal Handling in C

## Description

This project demonstrates how to handle POSIX signals in a multithreaded C program. It evolves across three parts: random signal handling (Part 1), inter-thread signal delivery (Part 2), and advanced signal masking and queue inspection (Part 3). The program uses multiple worker threads, each configured to handle a unique set of signals, and explores how signals are delivered, handled, ignored, or queued depending on thread-specific masking.

Each thread:

- Computes the sum from 0 to a target value with a 1-second delay per iteration.
- Handles only its assigned signals using a common signal handler.
- Ignores other signals by masking them at the thread level.

The main process:

- Spawns the worker threads and a signal generator thread (Part 1/2).
- Catches `SIGINT` (e.g. Ctrl+C) to exit cleanly.
- Blocks a group of signals in Part 3 and examines pending queues.

## Build Instructions

To build the Part 1/2 version:

```bash
make clean && make 
```
To run Parts 1/2 version:
```bash
./program
```

To build the Part 3 version:

```bash
make clean && make q3
```
To run Parts 1/2 version:
```bash
./program_q3
```

## Description
You’ll see:
- Each thread announces which signals it is handling.
- As signals are sent, threads log them as either caught or ignored.
- Threads may send signals to each other or to the main thread (Part 2).
- The main and each thread may display their pending signals (Part 3).
- Ctrl+C exits the program cleanly via SIGINT.

## Signals Used

The following 8 signals are part of the test pool:

- `SIGINT` (Ctrl+C)
- `SIGABRT`
- `SIGILL`
- `SIGCHLD`
- `SIGSEGV`
- `SIGFPE`
- `SIGHUP`
- `SIGTSTP`

Each thread handles a unique random set of 4 from these.

## Signal Generator

A dedicated thread in the main process (Part 1/2):

- Sends a random signal from the pool
- To a randomly selected thread
- Every 2 seconds

The generator prints both the signal number and the target thread’s ID.

## File Structure
.
├── main.c         # Entry point for Part 1/2
├── thread.c       # Thread logic for Part 1/2
├── main_q3.c      # Entry point for Part 3
├── thread_q3.c    # Thread logic for Part 3
├── utils.c        # Signal logic and generator
├── utils.h
├── Makefile
└── README.md

## Notes

- Signal delivery in POSIX threads is influenced by thread-specific signal masks.
- Signal handlers are shared across threads, but delivery is based on whether a signal is unblocked in the target thread.
- `pthread_kill` is used to target signals at specific threads.
- `sigpending()` is used in Part 3 to inspect blocked-but-undelivered signals.