#include "thread.h"
#include "utils.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

thread_info_t tinfo[NUM_THREADS];
pthread_t main_thread_id;

// SIGINT handler for main process
void handle_sigint(int signo) {
  printf("\nMain: Caught SIGINT (Ctrl+C). Exiting...\n");
  exit(0);
}

/* Main Program: initializes threads, sets up signal handling, and launches a
 * random number generator */
int main() {
  // Register SIGINT handler to allow clean Ctrl+C exit
  signal(SIGINT, handle_sigint);

  // Array to store metadata for each worker thread
  thread_info_t tinfo[NUM_THREADS];

  // Set up global signal handlers for all relevant signals
  setup_signal_handlers();

  // 
  main_thread_id = pthread_self();

  // Create NUM_THREADS worker threads, passing each its metadata pointer
  for (int i = 0; i < NUM_THREADS; ++i) {
    if (pthread_create(&tinfo[i].thread_id, NULL, thread_function, &tinfo[i]) !=
        0) {
      perror("Failed to create thread");
      exit(EXIT_FAILURE);
    }
  }

  // Create a signal generator thread to periodically send signals to random
  // threads
  pthread_t generator_thread;
  generator_args_t gen_args = {.tinfo = tinfo, .num_threads = NUM_THREADS};

  if (pthread_create(&generator_thread, NULL, generate_random_signals,
                     &gen_args) != 0) {
    perror("Failed to create signal generator thread");
    exit(EXIT_FAILURE);
  }

  // Wait for all worker threads to finish their computations
  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_join(tinfo[i].thread_id, NULL);
  }

  // Cancel and clean up the generator thread
  pthread_cancel(generator_thread);
  pthread_join(generator_thread, NULL);

  printf("All threads have completed their computations.\n");
  return 0;
}
