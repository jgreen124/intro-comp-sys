#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Run function and test with command `kill -SIGUSR1 <pid>`
void sighandler(int signum) {
  int got_sigusr1 = 1;
  fprintf(stderr, "got signal number = %d\n", signum);
  signal(signum, sighandler); // resintall handler for next use
}

int main() {
  printf("PID = %u\n", getpid());
  if ((signal(SIGUSR1, sighandler)) < 0) {
    fprintf(stderr, "couldn't establish SIGUSR handler");
    exit(1);
  }
  sleep(50);
}
