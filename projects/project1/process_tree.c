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

#define MAX_HIDDEN_KEYS 80

// The c2p struct is used to return results from child to parent
typedef struct {
    int max; // maximum value found in chunk
    double avg; // average value found in chunk
    int hidden_count; // number of hidden keys found
    int hidden_keys[MAX_HIDDEN_KEYS]; // array to store hidden keys
    int hidden_indices[MAX_HIDDEN_KEYS]; // array to store indices of hidden keys
    int total_count; // number of elements processed
    int sum; // total sum for weighted averaging
} c2p_data;

void bfs_search(int *number_arr, int start, int end, int H, int *pn_left, int return_arg, int fanout) {
    pid_t pid = getpid();
    pid_t parent_pid = getppid();

    // Base case: no more forks or chunk small enough
    if (*pn_left <= 1 || (end - start + 1) <= 1000 || fanout < 2) {
        printf("Hi I'm process %d with return arg %d and my parent is %d\n", pid, return_arg, parent_pid);
    // printf("Process %d started. Parent: %d | Range: [%d-%d], return_arg: %d, pn_left: %d\n", pid, parent_pid, start, end, return_arg, *pn_left);
    // printf("Process %d assigned range [%d, %d], return_arg: %d, fanout: %d\n", pid, start, end, return_arg, fanout);
    // printf("pn_left = %d\n", *pn_left);

        c2p_data c2pData = {.max = number_arr[start], .hidden_count = 0, .sum = 0, .total_count = end - start + 1};
        
        // for (int i = start; i <= end; i++) {
        //     printf("[TRACE] number_arr[%d] = %d\n", i, number_arr[i]);
        // }

        for (int i = start; i <= end; i++) {
            int val = number_arr[i];
            if (val < 0 && val >= -80 && c2pData.hidden_count < MAX_HIDDEN_KEYS) {
                c2pData.hidden_keys[c2pData.hidden_count] = val;
                c2pData.hidden_indices[c2pData.hidden_count] = i;
                c2pData.hidden_count++;
                printf("Hi I'm process %d with return arg %d, I found the hidden key %d in position %d\n",
                       pid, return_arg, val, i);
            }
            if (val > c2pData.max) c2pData.max = val;
            c2pData.sum += val;
        }

        c2pData.avg = (double)c2pData.sum / c2pData.total_count;

        printf("Process %d (return arg %d): Max=%d, Avg=%.2f, HiddenKeys=%d\n",
               pid, return_arg, c2pData.max, c2pData.avg, c2pData.hidden_count);

        // for (int i = 0; i < result.hidden_count; i++) {
        //     printf("Hidden key: %d at index %d\n", c2pData.hidden_keys[i], c2pData.hidden_indices[i]);
        // }

        // usleep(200000); // Sleep for 0.2 seconds
        return;
    }

    int total_length = end - start + 1;
    int chunk_size = total_length / fanout;
    c2p_data aggregated = {.max = number_arr[start], .hidden_count = 0, .sum = 0, .total_count = 0};

    for (int i = 0; i < fanout && *pn_left > 1; i++) {
        int chunk_start = start + i * chunk_size;
        int chunk_end = (i == fanout - 1) ? end : chunk_start + chunk_size - 1;
        // printf("Forking for chunk [%d - %d] (return_arg %d)\n", chunk_start, chunk_end, return_arg + i + 1);

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
            close(p2c_pipe[1]);
            close(c2p_pipe[0]);

            p2c_data p2cData;
            read(p2c_pipe[0], &p2cData, sizeof(p2c_data));
            close(p2c_pipe[0]);

            bfs_search(number_arr, p2cData.start, p2cData.end, p2cData.H, pn_left, p2cData.return_arg, fanout);

            c2p_data c2pData = {0};
            write(c2p_pipe[1], &c2pData, sizeof(c2p_data));
            close(c2p_pipe[1]);
            exit(p2cData.return_arg);
        } else {
            close(p2c_pipe[0]);
            close(c2p_pipe[1]);

            p2c_data p2cData = {
                .start = chunk_start,
                .end = chunk_end,
                .H = H,
                .return_arg = return_arg + i + 1
            };
            write(p2c_pipe[1], &p2cData, sizeof(p2c_data));
            close(p2c_pipe[1]);

            c2p_data child_result;
            read(c2p_pipe[0], &child_result, sizeof(c2p_data));
            close(c2p_pipe[0]);

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

    int parent_start = start + fanout * chunk_size;
    int parent_end = end;
    if (fanout == 1) parent_end = end;

    if (parent_start <= parent_end) {
        bfs_search(number_arr, parent_start, parent_end, H, pn_left, return_arg, fanout);
    }

    for (int i = 0; i < fanout && *pn_left + i < 9999; i++) {
        int status;
        wait(&status);
    }

    aggregated.avg = (double)aggregated.sum / aggregated.total_count;

    printf("Aggregated (pid %d): Max=%d, Avg=%.2f, HiddenKeys=%d\n",
           pid, aggregated.max, aggregated.avg, aggregated.hidden_count);
    for (int i = 0; i < aggregated.hidden_count; i++) {
        printf("Hidden Value: %d at Index: %d\n",
               aggregated.hidden_keys[i], aggregated.hidden_indices[i]);
    }
}
