#ifndef UTILS_H
#define UTILS_H

#include <pthread.h>
#include <signal.h>

// Declarations for signal handling
#define MAX_SIGNALS 8        // Max number of signals in the system
#define SIGNALS_PER_THREAD 4 // Each thread handles 4 of the possible signals

#define NUM_THREADS 4 // number of worker threads

// Store per-thread information
typedef struct {
  pthread_t thread_id;             // Thread ID
  pid_t tid;                       // Kernel thread ID
  int signals[SIGNALS_PER_THREAD]; // Array of signals a thread handles
} thread_info_t;

/**
 * Structure for passing arguments to the signal generator thread.
 */
// Store data to be passed to signal generator
typedef struct {
  thread_info_t *tinfo; // thread info struct
  int num_threads;      // number of threads
} generator_args_t;

// Global list of signals in the pool (e.g., SIGABRT, SIGILL, etc.)
extern int all_signals[];
extern const int NUM_SIGNALS;

// Store current thread's assigned signals
void set_thread_signal_set(int *signals, int count);

// Check if a signal is handled by current thread
int is_signal_handled_by_thread(int signo);

// Register custom signal handlers for each signal
void setup_signal_handlers();

// Random signal generator
void *generate_random_signals(void *arg);

#endif // UTILS_H
