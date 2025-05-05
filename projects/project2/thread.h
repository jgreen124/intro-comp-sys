#ifndef THREAD_H
#define THREAD_H

/**
 * @file thread.h
 * @brief Declaration for the thread worker function used in signal handling
 * test.
 *
 * Each thread:
 * - Selects and handles 4 of the 8 supported signals.
 * - Blocks the rest of the signals.
 * - Performs a dummy computation to allow time for signal delivery.
 */

/* Thread worker function used in signal handling test.
    - Select 4 of 8 signals to handle and block the rest
    - Perform long computation */
void *thread_function(void *arg);

#endif // THREAD_H
