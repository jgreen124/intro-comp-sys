#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "process_tree.h"

typedef struct {
    int start;
    int end;
    int H;
    int return_arg;
} WorkUnit;

void execute_bfs_node(int *data, int start, int end, int H, int *pn_left, int return_arg, int fanout) {
    pid_t my_pid = getpid();
    pid_t my_ppid = getppid();

    if (*pn_left <= 1 || (end - start + 1) <= 1000 || fanout < 2) {
        printf("Hi I'm process %d with return arg %d and my parent is %d\n", my_pid, return_arg, my_ppid);
        int hidden_count = 0, max = data[start], sum = 0;

        for (int i = start; i <= end; i++) {
            if (data[i] < 0 && data[i] >= -80) {
                printf("Hi I'm process %d with return arg %d, I found the hidden key %d in position %d\n",
                       my_pid, return_arg, data[i], i);
                hidden_count++;
            }
            if (data[i] > max) max = data[i];
            sum += data[i];
        }

        double avg = (double)sum / (end - start + 1);
        printf("Process %d (return arg %d): Max=%d, Avg=%.2f, HiddenKeys=%d\n",
               my_pid, return_arg, max, avg, hidden_count);
        return;
    }

    int total_length = end - start + 1;
    int chunk_size = total_length / fanout;

    for (int i = 0; i < fanout && *pn_left > 1; i++) {
        int chunk_start = start + i * chunk_size;
        int chunk_end = (i == fanout - 1) ? end : chunk_start + chunk_size - 1;

        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        (*pn_left)--;

        if (pid == 0) {
            // Child
            close(pipefd[1]);
            WorkUnit w;
            read(pipefd[0], &w, sizeof(WorkUnit));
            close(pipefd[0]);

            execute_bfs_node(data, w.start, w.end, w.H, pn_left, w.return_arg, fanout);
            exit(w.return_arg);
        } else {
            // Parent
            close(pipefd[0]);
            WorkUnit w = {
                .start = chunk_start,
                .end = chunk_end,
                .H = H,
                .return_arg = return_arg + i + 1
            };
            write(pipefd[1], &w, sizeof(WorkUnit));
            close(pipefd[1]);
        }
    }

    // Parent also handles its own chunk (first one)
    int parent_start = start;
    int parent_end = start + chunk_size - 1;
    if (fanout == 1) parent_end = end;  // Safety for bad values

    execute_bfs_node(data, parent_start, parent_end, H, pn_left, return_arg, fanout);

    // Wait for all children
    for (int i = 0; i < fanout && *pn_left + i < 9999; i++) {
        int status;
        wait(&status);
    }
}
