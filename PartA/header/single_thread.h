#define MSIZE 256
#pragma unroll

void mult(int N, int *matA, int *matB, int *output)
{
    // Iterate over first half of output elements
    for(int i = 0; i < MSIZE; i+=2) {
        int temp1 = 0;
        int temp2 = 0;
        int temp3 = 0;
        int temp4 = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < i + 1; j+=2) {
              int rowA = j;
              int colA = i - j;
              int rowB = i - j;
              int colB = MSIZE - j - 2;
              int indexA = rowA * N + colA;
              int indexB = rowB * N + colB;
              //Loop unrolling
              temp1 += matA[indexA] * matB[indexB+1];
              temp2 += matA[indexA+1] * matB[indexB+N+1];
              temp3 += matA[indexA+N] * matB[indexB];
              temp4 += matA[indexA+N+1] * matB[indexB+N];
            }
            //Adding the elements
            output[i] += temp1;
            output[i+1] += temp2+temp3;
            output[i+2] += temp4;
    }

    // Iterate over second half of output elements
    for(int i = MSIZE; i < 2 * MSIZE - 1 - 2; i+=2) {
        int temp1 = 0;
        int temp2 = 0;
        int temp3 = 0;
        int temp4 = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < 2 * MSIZE - (i + 1) - 2; j+=2) {
              int rowA = i + j - MSIZE + 2;
              int colA = MSIZE - j - 2;
              int rowB = MSIZE - j - 2;
              int colB = 2 * MSIZE - j - 2*2 - i;
              int indexA = rowA * N + colA;
              int indexB = rowB * N + colB;
              //Loop unrolling
              temp1 += matA[indexA] * matB[indexB+1];
              temp2 += matA[indexA+1] * matB[indexB+N+1];
              temp3 += matA[indexA+N] * matB[indexB];
              temp4 += matA[indexA+N+1] * matB[indexB+N];
            }
            //Adding the elements
            output[i] += temp1;
            output[i+1] += temp2+temp3;
            output[i+2] += temp4;
    }
}

// Optimize this function
void singleThread(int N, int *matA, int *matB, int *output)
{
    // Iterate over first half of output elements
    for(int i = 0; i < N; i += MSIZE) {
        // Iterate over diagonal elements
        for(int j = 0; j < i + 1; j += MSIZE) {
              int rowA = j;
              int colA = i - j;
              int rowB = i - j;
              int colB = N - j - MSIZE;
              mult(N, &matA[rowA * N + colA], &matB[rowB * N + colB], &output[i]);
          }
    }
    // Iterate over second half of output elements
    for(int i = N; i < 2 * N - 1 - MSIZE; i += MSIZE) {
        // Iterate over diagonal elements
        for(int j = 0; j < 2 * N - (i + 1) - MSIZE; j += MSIZE) {
              int rowA = i + j - N + MSIZE;
              int colA = N - j - MSIZE;
              int rowB = N - j - MSIZE;
              int colB = 2 * N - j - i - 2*MSIZE;
              mult(N, &matA[rowA * N + colA], &matB[rowB * N + colB], &output[i]);
        }
    }
}

