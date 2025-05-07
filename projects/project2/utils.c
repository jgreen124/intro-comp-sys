#include "utils.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

//#define NUM_THREADS 4  // moved here from main.c

// Define the list of signals to randomly assign and send (SIGINT intentionally
// excluded)
int all_signals[] = {SIGABRT, SIGILL, SIGCHLD, SIGSEGV,
                     SIGFPE,  SIGHUP, SIGTSTP};
const int NUM_SIGNALS = sizeof(all_signals) / sizeof(all_signals[0]);

// Thread-local storage for the current thread's handled signals
__thread int thread_signals[MAX_SIGNALS];
__thread int thread_signal_count = 0;

// Store signals a thread will handle
void set_thread_signal_set(int *signals, int count) {
  thread_signal_count = count;
  for (int i = 0; i < count; ++i) {
    thread_signals[i] = signals[i];
  }
}

// Check if signal is part of current thread's handled set
int is_signal_handled_by_thread(int signo) {
  for (int i = 0; i < thread_signal_count; ++i) {
    if (thread_signals[i] == signo) {
      return 1;
    }
  }
  return 0;
}

/* Custom signal handler
    - Exits program on SIGINT
    - Handles or ignores other signals based on the thread's assigned list
   (doesn't exit for testing purposes)
*/
void signal_handler(int signo) {
  pid_t tid = syscall(SYS_gettid);

  if (signo == SIGINT) {
    printf("Thread TID %d: Received SIGINT (Ctrl+C or SIGINT). Exiting...\n",
           tid);
    exit(0);
  } else if (is_signal_handled_by_thread(signo)) {
    printf("Thread TID %d: Caught signal %d\n", tid, signo);
  } else {
    printf("Thread TID %d: Ignored signal %d\n", tid, signo);
  }
}

// Set up the custom signal handler for all signals with SIGINT handled
// explicitly
void setup_signal_handlers() {
  struct sigaction sa;
  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  for (int i = 0; i < NUM_SIGNALS; ++i) {
    sigaction(all_signals[i], &sa, NULL);
  }

  // Always handle SIGINT to allow Ctrl+C shutdown
  sigaction(SIGINT, &sa, NULL);
}

// Send random signals to random threads
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
      printf("Main: Sent signal %d to thread (tid %d)\n", sig,
             tinfo[thread_index].tid);
    } else {
      fprintf(stderr, "Main: Failed to send signal %d to thread index %d\n",
              sig, thread_index);
    }

    sleep(2);
  }

  return NULL;
}
