#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "process_tree.h"

typedef struct {
    pid_t pid;
    int pipefd[2]; // pipefd[1] = write end (parent), pipefd[0] = read end (child)
} ChildInfo;

void create_bfs_process_tree(int PN, int fanout) {
    pid_t root_pid = getpid(); // Save original parent PID
    ChildInfo *children = malloc(sizeof(ChildInfo) * PN);
    int child_count = 0;

    for (int i = 0; i < PN; i++) {
        if (getpid() != root_pid) break; // Only root process forks

        // Create pipe for communication with child
        if (pipe(children[i].pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // CHILD PROCESS
            close(children[i].pipefd[1]); // Close write end
            printf("Hi I'm process %d (child %d of %d)\n", getpid(), i, getppid());

            // Simulate reading work from parent
            char buf[64];
            read(children[i].pipefd[0], buf, sizeof(buf));
            printf("Process %d received message: %s\n", getpid(), buf);

            close(children[i].pipefd[0]);
            sleep(3); // simulate work
            exit(i);  // return unique exit code
        } else {
            // PARENT
            children[i].pid = pid;
            close(children[i].pipefd[0]); // Close read end in parent

            char msg[64];
            snprintf(msg, sizeof(msg), "Hello child %d from parent %d", i, getpid());
            write(children[i].pipefd[1], msg, sizeof(msg));
            close(children[i].pipefd[1]);
        }

        child_count++;
    }

    // Wait for all children
    if (getpid() == root_pid) {
        for (int i = 0; i < child_count; i++) {
            int status;
            pid_t ended = wait(&status);
            printf("Parent %d: child %d exited with status %d\n",
                   getpid(), ended, WEXITSTATUS(status));
        }
    }

    free(children);
}
