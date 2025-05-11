#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sig_handler(int signum) {
  printf("\nReceived signal %d", signum);
} // Setup signal handler

int main() {
  signal(SIGINT, sig_handler); // Point sigint to sig_handler
  printf("Send a signal now\n");
  sleep(10);
  return 0;
}
