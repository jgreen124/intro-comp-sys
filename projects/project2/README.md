# Multithreaded Signal Handling in C

## Description

This project demonstrates how to handle POSIX signals in a multithreaded C program. It creates multiple worker threads, each configured to handle a unique set of 4 randomly assigned signals from a fixed pool of 8. A separate generator thread sends random signals to randomly selected threads to test the behavior.

Each thread:

- Computes the sum from 0 to `10 × tid` with a 1-second delay per iteration.
- Handles only its assigned signals using a common signal handler.
- Ignores all other signals by masking them.

The main process:

- Spawns the worker threads and the signal generator thread.
- Catches `SIGINT` (e.g. Ctrl+C) to exit cleanly.

## Build Instructions

Run the following commands:

```bash
make clean
make
```

This will compile the project into an executable named `program`.

## Run

```bash
./program
```

You’ll see:

- Each thread announces which signals it is handling.
- As signals are sent, each thread either logs them as caught or ignored.
- Ctrl+C exits the program cleanly via `SIGINT`.

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

A dedicated thread in the main process:

- Sends a random signal from the pool
- To a randomly selected thread
- Every 2 seconds

The generator prints both the signal number and the target thread’s ID.

## File Structure

```
.
├── main.c         # Entry point
├── thread.c       # Thread logic
├── thread.h
├── utils.c        # Signal logic and generator
├── utils.h
├── Makefile
└── README.md
```

## Clean

To remove compiled files:

```bash
make clean
```

## Notes

- Signal delivery in POSIX threads is influenced by signal masks.
- Signal handlers are shared across threads, but delivery is based on which threads have a signal unblocked.
- `pthread_kill` is used to target signals at specific threads.

---

Enjoy exploring low-level signal handling in a concurrent setting!
