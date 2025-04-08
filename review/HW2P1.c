#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
int p = fork();
for (int i = 0; i<=p; i++) {
    waitpid(-1, NULL, 0); 
    printf ("GrPr: %d, Pr: %d, num: %d, iter = %d\n", getppid(), getpid(), p, i);
    p = fork();
    }
}
