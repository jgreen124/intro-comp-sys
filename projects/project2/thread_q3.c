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

extern thread_info_t tinfo[];
extern pthread_t main_thread_id;
extern int all_signals[];
extern const int NUM_SIGNALS;

void check_pending_signals_thread(pid_t tid) {
  sigset_t pending;
  sigpending(&pending);
  printf("Thread TID %d pending signals:\n", tid);
  for (int i = 0; i < NUM_SIGNALS; ++i) {
    if (sigismember(&pending, all_signals[i])) {
      printf("  Signal %d is pending\n", all_signals[i]);
    }
  }
}

void *thread_function(void *arg) {
  thread_info_t *tinfo = (thread_info_t *)arg;
  pid_t tid = syscall(SYS_gettid);
  tinfo->tid = tid;
  printf("Thread TID %d: Starting computation.\n", tid);

  // Seed RNG
  srand(time(NULL) ^ tid);

  // Assign 4 unique signals to handle
  int selected[MAX_SIGNALS];
  memset(selected, 0, sizeof(selected));

  for (int i = 0; i < SIGNALS_PER_THREAD; ++i) {
    int index;
    do {
      index = rand() % NUM_SIGNALS;
    } while (selected[index]);
    selected[index] = 1;
    tinfo->signals[i] = all_signals[index];
  }

  // Setup masking based on thread index (even/odd)
  sigset_t mask;
  sigemptyset(&mask);
  if (tinfo->tid % 2 == 0) {
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTSTP);
  } else {
    for (int i = 0; i < NUM_SIGNALS; ++i) {
      int sig = all_signals[i];
      if (sig != SIGINT && sig != SIGQUIT && sig != SIGTSTP) {
        sigaddset(&mask, sig);
      }
    }
  }

  pthread_sigmask(SIG_BLOCK, &mask, NULL);
  set_thread_signal_set(tinfo->signals, SIGNALS_PER_THREAD);

  // Print handled signals
  printf("Thread TID %d: Handling signals: ", tid);
  for (int i = 0; i < SIGNALS_PER_THREAD; ++i) {
    printf("%d ", tinfo->signals[i]);
  }
  printf("\n");

  // Short computation loop for test
  int sum = 0;
  int end = 5;
  for (int i = 0; i <= end; ++i) {
    sum += i;
    sleep(1);
  }

  printf("Thread TID %d: Computed sum = %d\n", tid, sum);
  check_pending_signals_thread(tid);

  pthread_exit(NULL);
}
