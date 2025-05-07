#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Shared data structure
typedef struct {
  int limit;          // Maximum limit to check for primes
  bool *is_num_prime; // Array to store if number is prime - needs to be
                      // initialized to all true
} PrimeArray;

// Reverse the digits of a number
int reverse_num(int num) {
  int reversedNum = 0;
  while (num > 0) {
    reversedNum = reversedNum * 10 + (num % 10);
    num /= 10;
  }
  return reversedNum;
}

// Check if a number is prime
bool is_num_prime(int num) {
  if (num < 2)
    return false;
  for (int i = 2; i <= sqrt(num); i++)
    if (num % i == 0)
      return false;
  return true;
}

// Sieve of Eratosthenes Algorithm
void run_sieve(bool *A, int n) {
  for (int i = 2; i <= sqrt(n); i++) {
    if (A[i]) {
      for (int j = i * i; j <= n; j += i) {
        A[j] = false;
      }
    }
  }
}

// Check if reversed number is prime
bool is_reversed_prime(int num) {
  int reversedNum = reverse_num(num);
  return reversedNum != num && is_num_prime(reversedNum);
}

// Run sieve algorithm and print numbers
void *sieve_worker(void *arg) {
  // Initialize prime array
  PrimeArray *primeArray = (PrimeArray *)arg;

  // Run sieve algorithm
  run_sieve(primeArray->is_num_prime, primeArray->limit);

  // Iterate through array and print primes
  printf("Primes ≤ %d:\n", primeArray->limit);
  for (int i = 2; i <= primeArray->limit; i++) {
    if (primeArray->is_num_prime[i]) {
      printf("%d ", i);
    }
  }
  printf("\n");
  return NULL;
}

// Check if reversed number of a prime is prime
void *reversible_worker(void *arg) {
  // Initialize prime array
  PrimeArray *primeArray = (PrimeArray *)arg;

  // Check if each number is prime and its reverse is also prime
  printf("Reversible primes ≤ %d:\n", primeArray->limit);
  for (int i = 2; i <= primeArray->limit; i++) {
    if (primeArray->is_num_prime[i] && is_reversed_prime(i)) {
      printf("%d ", i);
    }
  }
  printf("\n");
  return NULL;
}

int main(int argc, char *argv[]) {
  // Function takes in one argument, the upper limit for prime numbers
  // Also can use help flags for usage
  if (argc != 2 || (strcmp(argv[1], "-h") == 0) ||
      (strcmp(argv[1], "--help") == 0)) {
    fprintf(stderr, "Usage: %s <n>\n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]); // Upper limit to check for primes, n > 1
  if (n <= 1) {
    fprintf(stderr, "Enter a number > 1.\n");
    return 1;
  }

  // Allocate space for prime array and set every index to true
  bool *is_num_prime = malloc((n + 1) * sizeof(bool));
  for (int i = 0; i <= n; i++)
    is_num_prime[i] = true;

  // Initialize prime array
  PrimeArray data = {.limit = n, .is_num_prime = is_num_prime};

  // Initialize threads
  pthread_t sieve_thread, reverse_thread;

  // Create first thread and wait for it to finish
  // This thread runs the sieve algorithm to find all primes up to n
  pthread_create(&sieve_thread, NULL, sieve_worker, &data);
  pthread_join(sieve_thread, NULL);

  // Create second thread and wait for it to finish
  // This thread checks if the reverse of a prime is also prime
  pthread_create(&reverse_thread, NULL, reversible_worker, &data);
  pthread_join(reverse_thread, NULL);

  // Free allocated memory and return
  free(is_num_prime);
  return 0;
}
