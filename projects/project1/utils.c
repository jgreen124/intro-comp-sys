// utils.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"


static void shuffle(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

void generate_input_file(const char *filename, int L, int H) {
    if (L < 20000) {
        fprintf(stderr, "Error: L must be at least 20,000.\n");
        exit(EXIT_FAILURE);
    }

    if (H < 40 || H > 80) {
        fprintf(stderr, "Error: H must be between 40 and 80.\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int total_positive = L - H;

    // Select and shuffle H unique negative integers from -1 to -80
    int all_negatives[80];
    for (int i = 0; i < 80; i++) all_negatives[i] = -(i + 1);
    shuffle(all_negatives, 80);

    int *hidden_keys = malloc(sizeof(int) * H);
    for (int i = 0; i < H; i++) hidden_keys[i] = all_negatives[i];
    shuffle(hidden_keys, H); // Optional: extra shuffle for placement randomness

    // Generate H unique positions for hidden keys
    int *positions = malloc(sizeof(int) * H);
    for (int i = 0; i < H; i++) {
        positions[i] = rand() % L;
        for (int j = 0; j < i; j++) {
            if (positions[i] == positions[j]) {
                i--; // Retry if duplicate
                break;
            }
        }
    }

    // Sort positions to insert in order
    for (int i = 0; i < H - 1; i++) {
        for (int j = i + 1; j < H; j++) {
            if (positions[i] > positions[j]) {
                int tmp = positions[i];
                positions[i] = positions[j];
                positions[j] = tmp;
            }
        }
    }

    // Write integers to file with hidden keys at given positions
    int pos_idx = 0, hidden_idx = 0, positive_written = 0;

    for (int i = 0; i < L; i++) {
        if (pos_idx < H && i == positions[pos_idx]) {
            fprintf(fp, "%d\n", hidden_keys[hidden_idx++]);
            pos_idx++;
        } else {
            fprintf(fp, "%d\n", rand() % 1000 + 1); // Positive int [1–1000]
            positive_written++;
        }
    }

    fclose(fp);
    free(hidden_keys);
    free(positions);

    printf("Successfully generated %s with %d total integers and %d negative integers\n", filename, L, H);
}


int *read_file(const char *filename, int size) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int *array = malloc(sizeof(int) * size);
    
    if (!array) {
        perror("malloc");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        if (fscanf(fp, "%d", &array[i]) != 1) {
            fprintf(stderr, "Error reading file at integer number %d\n", i);
            fclose(fp);
            free(array);
            exit(EXIT_FAILURE);
        }
    }

    fclose(fp);
    return array;
}
