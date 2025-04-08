// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "process_tree.h"

int main(int argc, char *argv[]) {
    if (argc == 2 && (strcmp(argv[1], "--help") == 0)) {
        printf("Usage: %s <L> <H> <PN>\n", argv[0]);
        printf("  L  = total number of integers (>= 20000)\n");
        printf("  H  = number of hidden keys (40 to 80)\n");
        printf("  PN = number of processes\n");
        return EXIT_SUCCESS;
    }

    if (argc != 4) {
        fprintf(stderr, "Invalid command usage: Run '%s --help' for usage\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Pull L, H, and PN from command line arguments
    int L = atoi(argv[1]);
    int H = atoi(argv[2]);
    int PN = atoi(argv[3]);

    // Generate input file
    const char *filename = "input.txt";    
    generate_input_file(filename, L, H);

    // Read and print first 10 integers from file
    int *data = read_file(filename, L);
    for (int i = 0; i < 10; i++) {
        printf("data[%d] = %d\n", i, data[i]);
    }
    free(data);

    // Create BFS-style process tree with fan-out = 3 (you can change this)
    printf("Creating BFS process tree with PN = %d...\n", PN);
    create_bfs_process_tree(PN, 3); // fanout = 3 for now

    return 0;
}
