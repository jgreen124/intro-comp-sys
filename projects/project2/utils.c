#include "utils.h"
#include "thread.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

int all_signals[] = {SIGINT,  SIGABRT, SIGILL, SIGCHLD,
                     SIGSEGV, SIGFPE,  SIGHUP, SIGTSTP};
const int NUM_SIGNALS = sizeof(all_signals) / sizeof(all_signals[0]);

__thread int thread_signals[MAX_SIGNALS];
__thread int thread_signal_count = 0;

void set_thread_signal_set(int *signals, int count) {
  thread_signal_count = count;
  for (int i = 0; i < count; ++i) {
    thread_signals[i] = signals[i];
  }
}

int is_signal_handled_by_thread(int signo) {
  for (int i = 0; i < thread_signal_count; ++i) {
    if (thread_signals[i] == signo) {
      return 1;
    }
  }
  return 0;
}

void signal_handler(int signo) {
  pid_t tid = syscall(SYS_gettid);
  if (is_signal_handled_by_thread(signo)) {
    printf("Thread TID %d: Caught signal %d\n", tid, signo);
  } else {
    printf("Thread TID %d: Ignored signal %d\n", tid, signo);
  }
}

void setup_signal_handlers() {
  struct sigaction sa;
  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  for (int i = 0; i < NUM_SIGNALS; ++i) {
    sigaction(all_signals[i], &sa, NULL);
  }
}

void *generate_random_signals(void *arg) {
  generator_args_t *args = (generator_args_t *)arg;
  thread_info_t *tinfo = args->tinfo;
  int num_threads = args->num_threads;

  srand(time(NULL));

  while (1) {
    int thread_index = rand() % num_threads;
    int signal_index = rand() % NUM_SIGNALS;
    int sig = all_signals[signal_index];

    int result = pthread_kill(tinfo[thread_index].thread_id, sig);
    if (result == 0) {
      printf("Main: Sent signal %d to thread index %d\n", sig, thread_index);
    } else {
      fprintf(stderr, "Main: Failed to send signal %d to thread index %d\n",
              sig, thread_index);
    }

    sleep(2); // Wait before sending the next signal
  }

  return NULL;
}
