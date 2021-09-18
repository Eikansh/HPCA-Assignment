
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
              int colB = MSIZE - j - 1;
              int indexA = rowA * N + colA;
              int indexB = rowB * N + colB;
              /*
               * Indexing into matrix : mat[rowA * N +colA]
               * index = rowA * N + colA
               */
              temp1 += matA[indexA] * matB[indexB];
              temp2 += matA[indexA+1] * matB[indexB+N];
              temp3 += matA[indexA+N] * matB[indexB-1];
              temp4 += matA[indexA+N+1] * matB[indexB+N-1];
              //printf("[(%d, %d), (%d, %d)], ", rowA, colA, rowB, colB);
            }
            //printf("\n");
            output[i] += temp1;
            output[i+1] += temp2+temp3;
            output[i+2] += temp4;
    }

    // Iterate over second half of output elements
    for(int i = MSIZE; i < 2 * MSIZE - 1; i+=2) {
        int temp1 = 0;
        int temp2 = 0;
        int temp3 = 0;
        int temp4 = 0;
        // Iterate over diagonal elements
        for(int j = 0; j < 2 * MSIZE - (i + 1); j+=2) {
              int rowA = i + 1 + j - MSIZE;
              int colA = MSIZE - j - 1;
              int rowB = MSIZE - j - 1;
              int colB = 2 * MSIZE - j - 2 - i;
              int indexA = rowA * N + colA;
              int indexB = rowB * N + colB;
              //temp += matA[rowA * N + colA] * matB[rowB * N + colB];
              temp1 += matA[indexA] * matB[indexB];
              temp2 += matA[indexA+1] * matB[indexB+N];
              temp3 += matA[indexA+N] * matB[indexB-1];
              temp4 += matA[indexA+N+1] * matB[indexB+N-1];
              //printf("[(%d, %d), (%d, %d)], ", rowA, colA, rowB, colB);
            }
            //printf("\n");
            output[i] += temp1;
            output[i+1] += temp2+temp3;
            output[i+2] += temp4;
    }
}
