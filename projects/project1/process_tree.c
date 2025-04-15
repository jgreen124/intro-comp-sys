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

#define MAX_HIDDEN_KEYS 100

// The c2p struct is used to return results from child to parent
typedef struct {
    int max;
    double avg;
    int hidden_count;
    int hidden_keys[MAX_HIDDEN_KEYS];
    int hidden_indices[MAX_HIDDEN_KEYS];
    int total_count; // number of elements processed
    int sum; // total sum for weighted averaging
} c2p_data;

void bfs_search(int *number_arr, int start, int end, int H, int *pn_left, int return_arg, int fanout) {
    pid_t pid = getpid();
    pid_t parent_pid = getppid();

    // Base case: no more forks or chunk small enough
    if (*pn_left <= 1 || (end - start + 1) <= 1000 || fanout < 2) {
        printf("Hi I'm process %d with return arg %d and my parent is %d\n", pid, return_arg, parent_pid);

        c2p_data result = {.max = number_arr[start], .hidden_count = 0, .sum = 0, .total_count = end - start + 1};

        for (int i = start; i <= end; i++) {
            int val = number_arr[i];
            if (val < 0 && val >= -80 && result.hidden_count < MAX_HIDDEN_KEYS) {
                result.hidden_keys[result.hidden_count] = val;
                result.hidden_indices[result.hidden_count] = i;
                result.hidden_count++;
                printf("Hi I'm process %d with return arg %d, I found the hidden key %d in position %d\n",
                       pid, return_arg, val, i);
            }
            if (val > result.max) result.max = val;
            result.sum += val;
        }

        result.avg = (double)result.sum / result.total_count;

        printf("Process %d (return arg %d): Max=%d, Avg=%.2f, HiddenKeys=%d\n",
               pid, return_arg, result.max, result.avg, result.hidden_count);

        usleep(200000); // Sleep for 0.2 seconds

        // In base case, return results to parent via stdout for now (leaf)
        // Actual use: child would write to c2p pipe
        // This section will only be used by forked children
        return;
    }

    // Split the array into fanout-sized (2 or 4) chunks
    int total_length = end - start + 1;
    int chunk_size = total_length / fanout;
    c2p_data aggregated = {.max = number_arr[start], .hidden_count = 0, .sum = 0, .total_count = 0};

    for (int i = 0; i < fanout && *pn_left > 1; i++) {
        int chunk_start = start + i * chunk_size;
        int chunk_end = (i == fanout - 1) ? end : chunk_start + chunk_size - 1;

        int p2c_pipe[2];
        int c2p_pipe[2];
        if (pipe(p2c_pipe) == -1 || pipe(c2p_pipe) == -1) {
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
            close(p2c_pipe[1]);
            close(c2p_pipe[0]);

            p2c_data w;
            read(p2c_pipe[0], &w, sizeof(p2c_data));
            close(p2c_pipe[0]);

            // Recursively compute results
            bfs_search(number_arr, w.start, w.end, w.H, pn_left, w.return_arg, fanout);

            // Compose child result (for test only: empty structure)
            c2p_data result = {0};
            write(c2p_pipe[1], &result, sizeof(c2p_data));
            close(c2p_pipe[1]);
            exit(w.return_arg);
        } else {
            // Parent process
            close(p2c_pipe[0]);
            close(c2p_pipe[1]);

            p2c_data w = {
                .start = chunk_start,
                .end = chunk_end,
                .H = H,
                .return_arg = return_arg + i + 1
            };
            write(p2c_pipe[1], &w, sizeof(p2c_data));
            close(p2c_pipe[1]);

            // Wait and read results
            c2p_data child_result;
            read(c2p_pipe[0], &child_result, sizeof(c2p_data));
            close(c2p_pipe[0]);

            // Aggregate child's result
            if (child_result.max > aggregated.max) aggregated.max = child_result.max;
            aggregated.sum += child_result.sum;
            aggregated.total_count += child_result.total_count;
            for (int j = 0; j < child_result.hidden_count && aggregated.hidden_count < MAX_HIDDEN_KEYS; j++) {
                aggregated.hidden_keys[aggregated.hidden_count] = child_result.hidden_keys[j];
                aggregated.hidden_indices[aggregated.hidden_count] = child_result.hidden_indices[j];
                aggregated.hidden_count++;
            }
        }
    }

    // Parent handles remainder
    int parent_start = start + fanout * chunk_size;
    int parent_end = end;
    if (fanout == 1) parent_end = end;

    if (parent_start <= parent_end) {
        bfs_search(number_arr, parent_start, parent_end, H, pn_left, return_arg, fanout);
    }

    // Wait for all children
    for (int i = 0; i < fanout && *pn_left + i < 9999; i++) {
        int status;
        wait(&status);
    }

    // Finalize own aggregated result
    aggregated.avg = (double)aggregated.sum / aggregated.total_count;

    printf("[ROOT or Internal Process %d] Aggregated: Max=%d, Avg=%.2f, HiddenKeys=%d\n",
           pid, aggregated.max, aggregated.avg, aggregated.hidden_count);
    for (int i = 0; i < aggregated.hidden_count; i++) {
        printf("[HIDDEN] Value: %d at Index: %d\n",
               aggregated.hidden_keys[i], aggregated.hidden_indices[i]);
    }
}
