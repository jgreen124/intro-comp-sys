#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>

// Shared data structure
typedef struct {
    int limit;
    bool *is_prime;
} PrimeData;

// Utility: Reverse the digits of a number
int reverse_digits(int num) {
    int reversed = 0;
    while (num > 0) {
        reversed = reversed * 10 + (num % 10);
        num /= 10;
    }
    return reversed;
}

// Check if a number is prime (needed for reversed numbers)
bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i <= sqrt(num); i++)
        if (num % i == 0) return false;
    return true;
}

// Thread 1: Sieve of Eratosthenes
void *sieve_worker(void *arg) {
    PrimeData *data = (PrimeData *)arg;
    int n = data->limit;
    bool *A = data->is_prime;

    for (int i = 2; i <= sqrt(n); i++) {
        if (A[i]) {
            for (int j = i * i; j <= n; j += i) {
                A[j] = false;
            }
        }
    }

    printf("Primes ≤ %d:\n", n);
    for (int i = 2; i <= n; i++) {
        if (A[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
    return NULL;
}

// Thread 2: Reversible primes
void *reversible_worker(void *arg) {
    PrimeData *data = (PrimeData *)arg;
    int n = data->limit;
    bool *A = data->is_prime;

    printf("Reversible primes ≤ %d:\n", n);
    for (int i = 2; i <= n; i++) {
        if (A[i]) {
            int rev = reverse_digits(i);
            if (rev != i && is_prime(rev)) {
                printf("%d ", i);
            }
        }
    }
    printf("\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 1) {
        fprintf(stderr, "Enter a number > 1.\n");
        return 1;
    }

    // Allocate boolean prime array
    bool *is_prime = malloc((n + 1) * sizeof(bool));
    for (int i = 2; i <= n; i++) is_prime[i] = true;

    PrimeData data = { .limit = n, .is_prime = is_prime };

    pthread_t sieve_thread, reverse_thread;

    // Launch sieve thread
    pthread_create(&sieve_thread, NULL, sieve_worker, &data);
    pthread_join(sieve_thread, NULL);  // Ensure sieve completes before using the data

    // Launch reversible prime thread
    pthread_create(&reverse_thread, NULL, reversible_worker, &data);
    pthread_join(reverse_thread, NULL);

    free(is_prime);
    return 0;
}
