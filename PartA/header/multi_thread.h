#include <pthread.h>

/*Uncomment below lines to run on intel i5
#define NBLOCKS 64
#define BSIZE (2 * (N) / (NBLOCKS))
#define MAX_THREADS 32*NBLOCKS
*/
#define NBLOCKS 4
#define BSIZE (2 * (N) / (NBLOCKS))
#define MAX_THREADS 4

struct thread_data {
    int N;
    int *matA;
    int *matB;
    int *output;
};

pthread_mutex_t lock;

void* multiply(void *args) {
    struct thread_data *data = (struct thread_data*) args;
    int N = data -> N;

    for(int i = 0; i < BSIZE; i+=2) {
        int temp1 = 0;
        int temp2 = 0;
        int temp3 = 0;
        int temp4 = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < i + 1; j+=2) {
              int rowA = j;
              int colA = i - j;
              int rowB = i - j;
              int colB = BSIZE - j - 2;
              int indexA = rowA * data -> N + colA;
              int indexB = rowB * data -> N + colB;
              // * Indexing into matrix : mat[rowA * N +colA]
              // * index = rowA * N + colA
              temp1 += data -> matA[indexA] * data -> matB[indexB+1];
              temp2 += data -> matA[indexA+1] * data -> matB[indexB+data -> N+1];
              temp3 += data -> matA[indexA+data -> N] * data -> matB[indexB];
              temp4 += data -> matA[indexA+data -> N+1] * data -> matB[indexB+data -> N];
            }

            pthread_mutex_lock(&lock); 
            data -> output[i] += temp1;
            data -> output[i+1] += temp2+temp3;
            data -> output[i+2] += temp4;
            pthread_mutex_unlock(&lock); 
    }

    // Iterate over second half of output elements
    for(int i = BSIZE; i < 2 * BSIZE - 1 - 2; i+=2) {
        int temp1 = 0;
        int temp2 = 0;
        int temp3 = 0;
        int temp4 = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < 2 * BSIZE - (i + 1) - 2; j+=2) {
              int rowA = i + j - BSIZE + 2;
              int colA = BSIZE - j - 2;
              int rowB = BSIZE - j - 2;
              int colB = 2 * BSIZE - j - 2*2 - i;
              int indexA = rowA * data -> N + colA;
              int indexB = rowB * data -> N + colB;
              temp1 += data -> matA[indexA] * data -> matB[indexB+1];
              temp2 += data -> matA[indexA+1] * data -> matB[indexB+data -> N+1];
              temp3 += data -> matA[indexA+data -> N] * data -> matB[indexB];
              temp4 += data -> matA[indexA+data -> N+1] * data -> matB[indexB+data -> N];
            }
            pthread_mutex_lock(&lock); 
            data -> output[i] += temp1;
            data -> output[i+1] += temp2+temp3;
            data -> output[i+2] += temp4;
            pthread_mutex_unlock(&lock); 

    }
    return NULL;
}


// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
    pthread_t threads[MAX_THREADS];
    struct thread_data args[MAX_THREADS];
    int index = 0, rc;

    // Iterate over first half of output elements
    for(int i = 0; i < N; i += BSIZE) {
        // Iterate over diagonal elements
        for(int j = 0; j < i + 1; j += BSIZE) {
              int rowA = j;
              int colA = i - j;
              int rowB = i - j;
              int colB = N - j - BSIZE;
              args[index].N = N;
              args[index].matA = &matA[rowA * N + colA];
              args[index].matB = &matB[rowB * N + colB];
              args[index].output = &output[i];
              rc = pthread_create(&threads[index], NULL, multiply, (void *)&args[index]);
              index++;
          }
    }

    for(int k = 0; k < index; k++)
          rc = pthread_join(threads[k], NULL);
    index = 0;

    // Iterate over second half of output elements
    for(int i = N; i < 2 * N - 1 - BSIZE; i += BSIZE) {
        // Iterate over diagonal elements
        for(int j = 0; j < 2 * N - (i + 1) - BSIZE; j += BSIZE) {
              int rowA = i + j - N + BSIZE;
              int colA = N - j - BSIZE;
              int rowB = N - j - BSIZE;
              int colB = 2 * N - j - i - 2*BSIZE;
              args[index].N = N;
              args[index].matA = &matA[rowA * N + colA];
              args[index].matB = &matB[rowB * N + colB];
              args[index].output = &output[i];
              rc = pthread_create(&threads[index], NULL, multiply, (void *)&args[index]);
              index++;
        }
    }
    for(int i = 0; i < index; i++ ) {
          rc = pthread_join(threads[i], NULL);
    }
}
