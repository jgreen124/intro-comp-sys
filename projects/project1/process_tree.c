#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "process_tree.h"

// The p2c struct is used to send data from a parent process to a child process
typedef struct {
    int start; // start index of chunk for child to analyze
    int end; // end index of chunk for child to analyze
    int H; // number of hidden keys
    int return_arg; // return argument for child process
} p2c_data;

void bfs_search(int *number_arr, int start, int end, int H, int *pn_left, int return_arg, int fanout) {
    pid_t pid = getpid();
    pid_t parent_pid = getppid();
    // printf("Process %d assigned range [%d, %d], return_arg: %d, fanout: %d\n", pid, start, end, return_arg, fanout);
    // printf("pn_left = %d\n", *pn_left);

    // Base case: no more forks or chunk small enough
    if (*pn_left <= 1 || (end - start + 1) <= 1000 || fanout < 2) {
        printf("Hi I'm process %d with return arg %d and my parent is %d\n", pid, return_arg, parent_pid);
        
        int hidden_count = 0, max = number_arr[start], sum = 0;
        // for (int i = start; i <= end; i++) {
        //     printf("[TRACE] number_arr[%d] = %d\n", i, number_arr[i]);
        // }

        // Process the chunk
        for (int i = start; i <= end; i++) {
            if (number_arr[i] < 0 && number_arr[i] >= -80) {
                printf("Hi I'm process %d with return arg %d, I found the hidden key %d in position %d\n",
                       pid, return_arg, number_arr[i], i);
                hidden_count++;
            }
            if (number_arr[i] > max) max = number_arr[i];
            sum += number_arr[i];
        }
        
        double avg = (double)sum / (end - start + 1);
        printf("Process %d (return arg %d): Max=%d, Avg=%.2f, HiddenKeys=%d\n",
               pid, return_arg, max, avg, hidden_count);

        usleep(200000); // Sleep for 0.2 seconds
        return;
    }

    int total_length = end - start + 1;
    int chunk_size = total_length / fanout;

    for (int i = 0; i < fanout && *pn_left > 1; i++) {
        int chunk_start = start + i * chunk_size;
        int chunk_end = (i == fanout - 1) ? end : chunk_start + chunk_size - 1;

        // printf("Process %d is creating child for chunk [%d, %d]\n", pid, chunk_start, chunk_end);

        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t child_pid = fork();
        if (child_pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        (*pn_left)--;

        if (child_pid == 0) {
            // Child process
            // printf("Child %d forked from parent %d\n", getpid(), getppid());
            close(pipefd[1]);
            p2c_data w;
            read(pipefd[0], &w, sizeof(p2c_data));
            close(pipefd[0]);

            bfs_search(number_arr, w.start, w.end, w.H, pn_left, w.return_arg, fanout);
            exit(w.return_arg);
        } else {
            // Parent process
            close(pipefd[0]);
            p2c_data w = {
                .start = chunk_start,
                .end = chunk_end,
                .H = H,
                .return_arg = return_arg + i + 1
            };
            write(pipefd[1], &w, sizeof(p2c_data));
            close(pipefd[1]);

            // printf("Parent %d sent work to child %d (range [%d, %d])\n", pid, child_pid, w.start, w.end);
        }
    }

    // Parent handles remainder
    int parent_start = start + fanout * chunk_size;
    int parent_end = end;    
    if (fanout == 1) parent_end = end;

    if (parent_start <= parent_end) {
        // printf("Process %d handling own chunk [%d, %d]\n", pid, parent_start, parent_end);
        bfs_search(number_arr, parent_start, parent_end, H, pn_left, return_arg, fanout);
    }

    // Wait for all children
    for (int i = 0; i < fanout && *pn_left + i < 9999; i++) {
        int status;
        pid_t finished = wait(&status);
        // printf("Parent %d waited for child %d, exit code %d\n", pid, finished, WEXITSTATUS(status));
    }
}
