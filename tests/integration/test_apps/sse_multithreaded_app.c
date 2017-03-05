/*! @file
 * This is a sample application using SSE floating-point arithmetic instructions
 * and multiple threads to test the concurrency of the ftrace application
 */

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/// Print the hex value of a 32-bit value to stdout
#define PRINT_HEX(fp) printf("%08x\n", *(uint32_t *)&(fp))
#define NUM_THREADS 8
#define TOTAL_COUNT 80000

void *calculcate_sum_part(void *unused);

/**
 * The main procedure of the application.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments
 */
int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, calculcate_sum_part, NULL);
  }
  volatile float sum = 0.0;
  for (int i = 0; i < NUM_THREADS; i++) {
    float *thread_result;
    pthread_join(threads[i], (void **)&thread_result);
    sum += *thread_result;
    free(thread_result);
  }
  PRINT_HEX(sum);

  return 0;
}

void *calculcate_sum_part(void *unused) {
  volatile float *sum_part = malloc(sizeof(float));
  for (int i = 0; i < (TOTAL_COUNT / NUM_THREADS); i++) {
    *sum_part += 1;
  }
  return (void *)sum_part;
}
