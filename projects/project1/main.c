#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include "utils.h"
#include "process_tree.h"

int main(int argc, char *argv[]) {
    // --help argument
    if (argc == 2 && (strcmp(argv[1], "--help") == 0)) {
        printf("Usage: %s <L> <H> <PN> <F>\n", argv[0]);
        printf("  L  = total number of integers (>= 20000)\n");
        printf("  H  = number of hidden keys (40 to 80)\n");
        printf("  PN = number of processes (max before core dump)\n");
        printf("  F  = number of children per process (2 or 4)\n");
        return EXIT_SUCCESS;
    }

    // Redirect stdout/stderr to output.txt for normal execution
    unlink("output.txt");
    int out_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0) {
        perror("open output.txt");
        return EXIT_FAILURE;
    }

    dup2(out_fd, STDOUT_FILENO);
    dup2(out_fd, STDERR_FILENO);
    close(out_fd);

    // Exit if there aren't 5 arguments
    if (argc != 5) {
        fprintf(stderr, "Invalid command usage. Run '%s --help' for usage\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse command-line arguments
    int L = atoi(argv[1]);
    int H = atoi(argv[2]);
    int PN = atoi(argv[3]);
    int fanout = atoi(argv[4]);

    // Check if arguments fall within required ranges
    if (L < 20000 || H < 40 || H > 80 || PN < 1 || (fanout != 2 && fanout != 4)) {
        fprintf(stderr, "Invalid arguments. Run '%s --help' for usage.\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Generate random text file
    const char *filename = "input.txt";
    generate_input_file(filename, L, H);
    int *data = read_file(filename, L);

    // Start timer
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    // RUN BFFS search
    printf("Running BFS tree: each child forks %d childs...\n", fanout);
    execute_bfs_node(data, 0, L - 1, H, &PN, 0, fanout);
    

    // End timer
    gettimeofday(&end_time, NULL);
    double elapsed = (end_time.tv_sec - start_time.tv_sec) +
                     (end_time.tv_usec - start_time.tv_usec) / 1e6;

    printf("For a list of %d numbers, the search took %.6f seconds.\n", L, elapsed);

    free(data);
    return 0;
}
