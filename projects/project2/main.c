#include "thread.h"
#include "utils.h"
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 4

// Handle Ctrl+C (SIGINT) in the main thread
void handle_sigint(int signo) {
  printf("\nMain: Caught SIGINT (Ctrl+C). Exiting...\n");
  exit(0);
}

int main() {
  signal(SIGINT, handle_sigint); // Register handler

  thread_info_t tinfo[NUM_THREADS];

  // Set up signal handlers
  setup_signal_handlers();

  // Create worker threads
  for (int i = 0; i < NUM_THREADS; ++i) {
    if (pthread_create(&tinfo[i].thread_id, NULL, thread_function, &tinfo[i]) !=
        0) {
      perror("Failed to create thread");
      exit(EXIT_FAILURE);
    }
  }

  // Start the signal generator in a separate thread
  pthread_t generator_thread;
  generator_args_t gen_args = {.tinfo = tinfo, .num_threads = NUM_THREADS};
  if (pthread_create(&generator_thread, NULL, generate_random_signals,
                     &gen_args) != 0) {
    perror("Failed to create signal generator thread");
    exit(EXIT_FAILURE);
  }

  // Wait for worker threads to finish
  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_join(tinfo[i].thread_id, NULL);
  }

  // Optionally, cancel the signal generator thread if needed
  pthread_cancel(generator_thread);
  pthread_join(generator_thread, NULL);

  printf("All threads have completed their computations.\n");
  return 0;
}
