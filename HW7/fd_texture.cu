#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

const int threadsPerBlock = 256;

// declare constant memory on the device
__device__ static int dev_N;
__device__ static double dev_dx;
// Declare the texture memory using int2 type
texture<int2, cudaTextureType1D, cudaReadModeElementType> tex_u;

// Declare the kernel function
__global__ void diff(double* du);

/* 
   int main(int argc, char* argv[])
   Demonstrate a simple example for implementing a 
   parallel finite difference operator
  
   Inputs: argc should be 2
   argv[1]: Length of the vector of data
  
   Outputs: the initial data and its derivative.
*/

int main(int argc, char* argv[]) {

  // read in the number of grid points
  int N = atoi(argv[1]);
	
  // determine how many blocks are needed for the whole grid
  const int blocksPerGrid = N/threadsPerBlock 
    + (N%threadsPerBlock > 0 ? 1 : 0);
		
  // allocate host memory
  double* u = (double*)malloc(N*sizeof(double));
  double* du = (double*)malloc(N*sizeof(double));

  double* dev_u;
  double* dev_du;

  // allocate device memory
  cudaMalloc((void**)&dev_u, N*sizeof(double));
  cudaMalloc((void**)&dev_du, N*sizeof(double));

  // initialize the data on the host
  double dx = 2*M_PI/N;
  for (int i=0; i<N; ++i)
    u[i] = sin(i*dx);

  // set the values of N and dx in constant memory
  cudaMemcpyToSymbol(dev_N, &N, sizeof(int));
  cudaMemcpyToSymbol(dev_dx, &dx, sizeof(double));
	
  // copy the input data to the device
  cudaMemcpy(dev_u, u, N*sizeof(double), cudaMemcpyHostToDevice);

  // The device memory is bound to the texture memory here.
  cudaBindTexture(NULL, tex_u, dev_u, N*sizeof(double));

  // call the kernel
  diff<<<blocksPerGrid, threadsPerBlock>>>(dev_du);

  // Copy the results back to the host
  cudaMemcpy(du, dev_du, N*sizeof(double), cudaMemcpyDeviceToHost);

  // The clean-up phase also requires the texture memory to be unbound.
  cudaUnbindTexture(tex_u);
  cudaFree(dev_u);
  cudaFree(dev_du);
  free(u);
  free(du);
  return 0;
}

__global__ void diff(double* du) {

  int g_i = (threadIdx.x + blockIdx.x * blockDim.x) % dev_N;
  int g_im = (g_i + dev_N - 1) % dev_N;
  int g_ip = (g_i + 1) % dev_N;

  // Fetch the data masquerading as int2
  int2 up_int2 = tex1Dfetch(tex_u, g_ip);
  int2 um_int2 = tex1Dfetch(tex_u, g_im);

  // Convert the int2 data into double
  double up = __hiloint2double(up_int2.y, up_int2.x);
  double um = __hiloint2double(um_int2.y, um_int2.x);

  du[g_i] = (up-um)/dev_dx/2.;
}
