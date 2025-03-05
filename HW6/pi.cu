#include <stdio.h>

#define NLOOP 1                                           // Number of iterations
#define NPTS 1000000000                                   // Number of points
#define NUM_THREAD  256                                   // Number of threads per block 
#define NUM_BLOCK  (NPTS + NUM_THREAD - 1) / NUM_THREAD   // Number of blocks per grid

int tid;
double pi = 0;

// Kernel on the GPU (device)
__global__ void compute_pi(float *sum) {

        unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;  // Sequential thread index across the blocks
        if (idx < NPTS)
        sum[idx] = 0.5/((idx - 0.75)*(idx - 0.25));
}

// The applicaiton on the CPU (host)
int main(int argc, char* argv[]) {

        double flops;

        int blocksPerGrid = (NPTS + NUM_THREAD - 1) / NUM_THREAD;   // Compute number of blocks needed
        float *sumHost, *sumDev;                                    // Pointer to host & device arrays

        size_t size = blocksPerGrid*NUM_THREAD*sizeof(float);       // Array memory size
        sumHost = (float *)malloc(size);                            // Allocate array on host
        cudaMalloc((void **) &sumDev, size);                        // Allocate array on device
        cudaMemset(sumDev, 0, size);                                // Initialize array in device to 0

        // Do calculation on device
        compute_pi <<<blocksPerGrid, NUM_THREAD>>> (sumDev);        // call CUDA kernel
        // Retrieve result from device and store it in host array
        cudaMemcpy(sumHost, sumDev, size, cudaMemcpyDeviceToHost);
        // Reduce
        for(tid=1; tid < NPTS; tid++)
                pi += sumHost[tid];

        printf("Processes = %d: %d Blocks and %d Threads per block\n", blocksPerGrid*NUM_THREAD, blocksPerGrid, NUM_THREAD);
        printf("NPTS = %d, NLOOP = %d, pi = %.16e\n", NPTS, NLOOP, pi/NLOOP);

        // Cleanup
        free(sumHost);
        cudaFree(sumDev);

        return (0);
}

