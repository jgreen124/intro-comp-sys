#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void chldHandler() {
  int pid1, status;
  pid1 = wait(&status);
  printf("child done in time\n");
  exit(status);
}

int main(int argc, char *argv[]) {
  int pid;
  signal(SIGCHLD, chldHandler); // Register chldHandler
  pid = fork();                 // fork

  if (!pid) {                  // Child runs this code
    execvp(argv[2], &argv[2]); // Sends SIGCHLD after running execvp
  } else {                     // Parent runs this
    sleep(10);
    printf("child too slow\n");
    kill(pid, SIGINT);
  }
}
