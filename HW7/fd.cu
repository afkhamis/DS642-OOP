#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

/*
  void diff(double* u, int* N, double* dx, double* du)
  
  Compute the central difference operator on periodic data
  
  Inputs: 
  double* u: Function data, assumed periodic
  int* N: pointer to the length of the data array
  double* dx: pointer to the space step size

  Outputs:
  double* du: central difference of the u data
*/
__global__ void diff(double* u, int* N, double* dx, double* du) {
  // blockIdx is a CUDA provided constant that tells 
  //      the block index within the grid
  int tid = blockIdx.x;
  // Notice there's no loop, each core will perform its operation on
  //      its own entry but some cores should not participate if they 
  //      are outside the range.
  if (tid < *N) {
    int ip = (tid+1)%(*N);
    int im = (tid+*N-1)%(*N);
    du[tid] = (u[ip]-u[im])/(*dx)/2.;
  }
}

/* 
   int main(int argc, char* argv[])
   Demonstrate a simple example for implementing a 
   parallel finite difference operator
  
   Inputs: argc should be 2
   argv[1]: Length of the vector of data
  
   Outputs: the initial data and its derivative.
*/
int main(int argc, char* argv[]) {
  int N = atoi(argv[1]); // Get the length of the vector from input
	
  // These are addresses into host memory
  double* u = (double*)malloc(N*sizeof(double)); // function data 
  double* du = (double*)malloc(N*sizeof(double)); // derivative data
	
  // These are addresses into device memory, the "dev_" is optional
  double* dev_u;	// function data
  double* dev_du; // derivative data
  double* dev_dx; // space step size
  int* dev_N; // array length
	
  // Allocate memory on the device 
  cudaMalloc((void**)&dev_u, N*sizeof(double));
  cudaMalloc((void**)&dev_du, N*sizeof(double));
  cudaMalloc((void**)&dev_dx, sizeof(double));
  cudaMalloc((void**)&dev_N, sizeof(int));

  // Initialize the function data on the host
  double dx = 2*M_PI/N;
  for (int i=0; i<N; ++i)
    u[i] = sin(i*dx);
		
  // copy the input data from the host to the device
  cudaMemcpy(dev_dx, &dx, sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(dev_u, u, N*sizeof(double), cudaMemcpyHostToDevice);
  cudaMemcpy(dev_N, &N, sizeof(int), cudaMemcpyHostToDevice);
	
  // execute the finite difference kernel using N blocks
  diff<<<N, 1>>>(dev_u, dev_N, dev_dx, dev_du);

  // copy the result from the device back to the host.
  cudaMemcpy(du, dev_du, N*sizeof(double), cudaMemcpyDeviceToHost);

  for (int i=0; i<N; ++i)
    printf("%lf\t%lf\n", u[i], du[i]);

  // clean up all the allocated memory
  cudaFree(dev_u);
  cudaFree(dev_du);
  cudaFree(dev_dx);
  cudaFree(dev_N);
  free(u);
  free(du);
  return 0;
}
