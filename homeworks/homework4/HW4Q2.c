#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

int pg[2];

void f1(int x, int y) {
    printf("[PID %d] Entering f1(%d, %d), sleeping for 1000 seconds...\n", getpid(), x, y);
    sleep(50);
}

void handler(int signum) {
    printf("[PID %d] Received signal %d, exiting with code 4.\n", getpid(), signum);
    exit(4);
}

int main(int argc, char* argv[]) {
    int i, n;
    char c[10];
    pid_t pid[5]; // Adjusted size to accommodate 5 child PIDs

    signal(SIGUSR1, handler);
    pipe(pg);

    for (i = 0; i < 5; i++) {
        pid[i] = fork();
        if (pid[i] == 0) {
            // Child process
            printf("[Child PID %d] Started (i = %d)\n", getpid(), i);
            sleep(3);
            int bytes_to_read = 2 * i + 1;
            printf("[Child PID %d] Attempting to read %d bytes from pipe...\n", getpid(), bytes_to_read);
            n = read(pg[0], c, bytes_to_read);
            printf("[Child PID %d] Read returned %d bytes.\n", getpid(), n);

            if (i < 3) {
                printf("[Child PID %d] Exiting with code %d.\n", getpid(), 2 - i);
                exit(2 - i);
            } else {
                f1(i, n);
                // This point is not reached as f1() sleeps indefinitely
            }
        }
    }

    // Parent process
    printf("[Parent PID %d] Sending SIGUSR1 to child PID %d.\n", getpid(), pid[3]);
    kill(pid[3], SIGUSR1);

    for (i = 0; i < 5; i++) {
        system("pstree -p -s $$"); // Display the process tree leading to the current process
        int status;
        pid_t wpid = wait(&status);
        int exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        printf("[Parent PID %d] Child PID %d terminated with exit code %d.\n", getpid(), wpid, exit_code);
        write(pg[1], c, exit_code);
    }

    printf("[Parent PID %d] All children have terminated. Displaying process tree:\n", getpid());
    system("pstree -p -s $$"); // Display the process tree leading to the current process

    f1(i, -1);
    return 0;
}
