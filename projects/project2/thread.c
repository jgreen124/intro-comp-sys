#include "thread.h"
#include "utils.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// External declarations for the global signal set and its size
extern int all_signals[];
extern const int NUM_SIGNALS;

// Thread function: selects random signals to handle, blocks the others, and
// does computation
void *thread_function(void *arg) {
  // Cast input argument to thread_info_t
  thread_info_t *tinfo = (thread_info_t *)arg;

  // Get and store kernel thread ID for consistent identification
  pid_t tid = syscall(SYS_gettid);
  tinfo->tid = tid;
  printf("Thread TID %d: Starting computation.\n", tid);

  // Seed RNG uniquely for each thread to ensure different signal assignments
  srand(time(NULL) ^ tid);

  // Track which signals have already been selected
  int selected[NUM_SIGNALS];
  memset(selected, 0, sizeof(selected));

  // Randomly assign SIGNALS_PER_THREAD unique signals
  for (int i = 0; i < SIGNALS_PER_THREAD; ++i) {
    int index;
    do {
      index = rand() % NUM_SIGNALS;
    } while (selected[index]); // Avoid duplicates
    selected[index] = 1;
    tinfo->signals[i] = all_signals[index];
  }

  // Block all signals initially
  sigset_t mask;
  sigemptyset(&mask);
  for (int i = 0; i < NUM_SIGNALS; ++i) {
    sigaddset(&mask, all_signals[i]);
  }
  pthread_sigmask(SIG_BLOCK, &mask, NULL);

  // Unblock only the signals this thread is meant to handle
  for (int i = 0; i < SIGNALS_PER_THREAD; ++i) {
    sigdelset(&mask, tinfo->signals[i]);
  }
  pthread_sigmask(SIG_SETMASK, &mask, NULL);

  // Store the signal list for handler access
  set_thread_signal_set(tinfo->signals, SIGNALS_PER_THREAD);

  // Print which signals this thread is set to handle
  printf("Thread TID %d: Handling signals: ", tid);
  for (int i = 0; i < SIGNALS_PER_THREAD; ++i) {
    printf("%d ", tinfo->signals[i]);
  }
  printf("\n");

  // Perform a long-running computation: sum from 0 to 10 * tid
  // Sleep 1 second per iteration to allow signal delivery
  int sum = 0;
  int end = 10 * tid;
  for (int i = 0; i <= end; ++i) {
    sum += i;
    sleep(1);
  }

  // Final output after computation completes (optional for testing)
  printf("Thread TID %d: Computed sum from 0 to %d = %d\n", tid, end, sum);
  pthread_exit(NULL);
}
