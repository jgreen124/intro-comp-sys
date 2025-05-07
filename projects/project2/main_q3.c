// =======================
// File: main_q3.c
// =======================

#include "thread.h"
#include "utils.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

thread_info_t tinfo[NUM_THREADS];
pthread_t main_thread_id;

void handle_sigint(int signo) {
  printf("\nMain: Caught SIGINT (Ctrl+C). Exiting...\n");
  exit(0);
}

void check_pending_signals(const char *label) {
  sigset_t pending;
  sigpending(&pending);
  printf("%s pending signals:\n", label);
  for (int i = 0; i < NUM_SIGNALS; ++i) {
    if (sigismember(&pending, all_signals[i])) {
      printf("  Signal %d is pending\n", all_signals[i]);
    }
  }
}

int main() {
  sigset_t parent_mask;
  sigemptyset(&parent_mask);
  sigaddset(&parent_mask, SIGINT);
  sigaddset(&parent_mask, SIGQUIT);
  sigaddset(&parent_mask, SIGTSTP);
  pthread_sigmask(SIG_BLOCK, &parent_mask, NULL);

  // Set up signal handlers
  setup_signal_handlers();
  signal(SIGINT, handle_sigint);

  main_thread_id = pthread_self();

  // Send each signal 3 times to parent BEFORE forking threads
  for (int s = 0; s < NUM_SIGNALS; ++s) {
    for (int j = 0; j < 3; ++j) {
      kill(getpid(), all_signals[s]);
    }
  }

  check_pending_signals("Main (before thread spawn)");

  // Create worker threads
  for (int i = 0; i < NUM_THREADS; ++i) {
    if (pthread_create(&tinfo[i].thread_id, NULL, thread_function, &tinfo[i]) != 0) {
      perror("Failed to create thread");
      exit(EXIT_FAILURE);
    }
  }

  // Wait for all threads to finish
  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_join(tinfo[i].thread_id, NULL);
  }

  check_pending_signals("Main (after threads join)");
  printf("All threads have completed their computations.\n");
  return 0;
}
