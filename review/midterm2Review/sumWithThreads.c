#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum;                   // This is shared by threads
void *runner(void *param); // Threads call this function

void *runner(void *param) {
  int i, upper = atoi(param);
  sum = 0;

  for (i = 1; i <= upper; i++) {
    sum += i;
  }
  pthread_exit(0);
}

int main(int argc, char *argv[]) {
  pthread_t tid;       // Thread identifier, same idea as PID
  pthread_attr_t attr; // Set the thread attributes

  if (argc != 2) {
    fprintf(stderr, "usage: ./sumWithThreads <int> \n");
    return -1;
  }
  if (atoi(argv[1]) < 0) {
    fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
  }

  pthread_attr_init(&attr); // Set default attributes for thread
  pthread_create(&tid, &attr, runner,
                 argv[1]); // Create thread, store tid, set default attributes,
                           // run runner with argv[1] as argument
  pthread_join(tid, NULL); // Wait for thread to finish

  printf("sum = %d", sum);
}
