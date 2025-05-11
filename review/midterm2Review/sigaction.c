#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void my_sig_handler(int signo) { printf("I received signal %d\n", signo); }

int main() {
  printf("my pid = %d\n", getpid());
  struct sigaction my_params, my_old_params;
  int success;

  my_params.sa_handler = my_sig_handler; // pointer to signal handler function
  my_params.sa_flags = 0;                // set flags, 0 is default behavior
  sigemptyset(
      &my_params.sa_mask); // sa_mask sets signals to be blocked, sigemptyset
                           // sets sa_mask to empty (no signals excluded)
  success = sigaction(SIGUSR1, &my_params,
                      &my_old_params); // set SIGUSR1 handler to my_sig_handler,
                                       // save old behavior to &my_old_params
  while (1) {
    sleep(1);
    printf("Program is in fact programming\n");
  }
}
