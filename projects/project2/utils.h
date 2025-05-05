#ifndef UTILS_H
#define UTILS_H

#include <pthread.h>
#include <signal.h>

#define MAX_SIGNALS 8
#define SIGNALS_PER_THREAD 4

typedef struct {
  pthread_t thread_id;
  int signals[SIGNALS_PER_THREAD];
} thread_info_t;

typedef struct {
  thread_info_t *tinfo;
  int num_threads;
} generator_args_t;

extern int all_signals[];
extern const int NUM_SIGNALS;

void set_thread_signal_set(int *signals, int count);
int is_signal_handled_by_thread(int signo);
void setup_signal_handlers();
void *generate_random_signals(void *arg);

#endif // UTILS_H
