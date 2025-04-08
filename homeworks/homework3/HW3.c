#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int pipefd[2];
    pid_t res;
    char ch[50];
    int r1, r2, r3, r4, r5, r6, r7;
    int status;
    pipe(pipefd);
    res = fork();

    if (res > 0)
    {
        r4 = res;
        sleep(5); // line A
        r1 = write(pipefd[1], &r4, 2 * sizeof(int));
        sleep(10); // line B
        r2 = write(pipefd[1], &r4, r1);
        printf("Finished!");
        r7 = waitpid(-1, &status, WUNTRACED | WNOHANG);
        close(pipefd[1]);
    }
    else if (res == 0)
    {
        close(pipefd[1]);
        r3 = read(pipefd[0], ch, 2 * sizeof(char));
        r5 = read(pipefd[0], &r4, sizeof(int));
        sleep(5); // line C
        r6 = read(pipefd[0], ch, 3 * sizeof(char));
        close(pipefd[0]);
        sleep(20); // line D
    }
    exit(0);
}
