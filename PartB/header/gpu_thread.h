// Create other necessary functions here
__global__ void gpu_dmm(int *matA, int *matB, int *output, int N) 
{
    int temp = 0; 
    for(int j = 0; j < threadIdx.x + 1; ++j) {
        int rowA = j;
        int colA = threadIdx.x - j;
        int rowB = threadIdx.x - j;
        int colB = N - j - 1;
        temp += matA[rowA * N + colA] * matB[rowB * N + colB];
    }
    output[threadIdx.x] = temp;
    temp = 0;
    int k = N + threadIdx.x;
    for(int j = 0; j < 2 * N - (k + 1); ++j) {
        int rowA = k + 1 + j - N;
        int colA = N - j - 1;
        int rowB = N - j - 1;
        int colB = 2 * N - j - 2 - k;
        temp += matA[rowA * N + colA] * matB[rowB * N + colB];
    }
    output[k] = temp;
}


// Fill in this function
void gpuThread(int N, int *matA, int *matB, int *output)
{
  int *gpu_a, *gpu_b, *gpu_out;
  cudaMalloc((void **) &gpu_a, sizeof(int)*N*N);
  cudaMalloc((void **) &gpu_b, sizeof(int)*N*N);
  cudaMalloc((void **) &gpu_out, sizeof(int)*2*N);
  
  // copy matrix A and B from host to device memory
  cudaMemcpy(gpu_a, matA, sizeof(int)*N*N, cudaMemcpyHostToDevice);
  cudaMemcpy(gpu_b, matB, sizeof(int)*N*N, cudaMemcpyHostToDevice);

  unsigned int grid_dim = 1;
  dim3 dimGrid(grid_dim);
  dim3 dimBlock(N);

  gpu_dmm<<<dimGrid, dimBlock>>>(gpu_a, gpu_b, gpu_out, N);    

  cudaMemcpy(output, gpu_out, sizeof(int)*2*N, cudaMemcpyDeviceToHost);
  cudaThreadSynchronize();
}
