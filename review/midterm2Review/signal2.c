#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void signal_callback_handler(int signum) {
  printf("\nCaught signal %d\n", signum);
  exit(signum);
}

int main() {
  signal(SIGINT, signal_callback_handler);

  while (1) {
    printf("Program is in fact programming\n");
    sleep(1);
  }
  return EXIT_SUCCESS;
}
