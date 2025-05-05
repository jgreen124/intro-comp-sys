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

extern int all_signals[];
extern const int NUM_SIGNALS;

void *thread_function(void *arg) {
  thread_info_t *tinfo = (thread_info_t *)arg;
  pid_t tid = syscall(SYS_gettid);
  printf("Thread TID %d: Starting computation.\n", tid);

  // Seed the random number generator
  srand(time(NULL) ^ tid);

  // Randomly select SIGNALS_PER_THREAD signals for this thread
  int selected[NUM_SIGNALS];
  memset(selected, 0, sizeof(selected));

  for (int i = 0; i < SIGNALS_PER_THREAD; ++i) {
    int index;
    do {
      index = rand() % NUM_SIGNALS;
    } while (selected[index]);
    selected[index] = 1;
    tinfo->signals[i] = all_signals[index];
  }

  // Set up the signal mask
  sigset_t mask;
  sigemptyset(&mask);
  for (int i = 0; i < NUM_SIGNALS; ++i) {
    sigaddset(&mask, all_signals[i]);
  }
  pthread_sigmask(SIG_BLOCK, &mask, NULL);

  // Unblock the selected signals
  for (int i = 0; i < SIGNALS_PER_THREAD; ++i) {
    sigdelset(&mask, tinfo->signals[i]);
  }
  pthread_sigmask(SIG_SETMASK, &mask, NULL);

  // Store the thread's signal set
  set_thread_signal_set(tinfo->signals, SIGNALS_PER_THREAD);

  // Print the signals this thread will handle
  printf("Thread TID %d: Handling signals: ", tid);
  for (int i = 0; i < SIGNALS_PER_THREAD; ++i) {
    printf("%d ", tinfo->signals[i]);
  }
  printf("\n");

  // Compute the sum from 0 to 10 × TID
  int sum = 0;
  int end = 10 * tid;
  for (int i = 0; i <= end; ++i) {
    sum += i;
    sleep(1); // Allow signal delivery
  }

  printf("Thread TID %d: Computed sum from 0 to %d = %d\n", tid, end, sum);
  pthread_exit(NULL);
}
