#include <stdio.h>
#include <cufft.h>  // This is the header file for the CUDA FFT library

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

__global__ void rescale(cufftDoubleComplex* a, double f);

/*
  int main(int argc, char* argv[])

  The main program takes no arguments and creates a data set to run 
  through the FFT server and outputs the results from doing a forward
  and inverse transform.

  Inputs: none

  Output: Sample output of the FFT server
*/

int main(int argc, char* argv[]) {

  // Number of collocation points in the data set
  const int N = 256;
  // Host memory for initializing the input data and retrieving the 
  //      output
  cufftDoubleComplex *u 
    = (cufftDoubleComplex*)malloc(N*sizeof(cufftDoubleComplex));
  cufftDoubleComplex *a 
    = (cufftDoubleComplex*)malloc(N*sizeof(cufftDoubleComplex));
	
  double dx = 2*M_PI/N;	// space step size

  // Initialize the data
  int i;
  for (int i=0; i<N; ++i) {
    u[i].x = sin(i*dx);
    u[i].y = 0.;
  }

  // Create the device memory for running the transform
  cufftDoubleComplex* dev_u;
  cufftDoubleComplex* dev_a;
  cudaMalloc((void**)&dev_u, sizeof(cufftDoubleComplex)*N);
  cudaMalloc((void**)&dev_a, sizeof(cufftDoubleComplex)*N);
	
  // Copy the input data to device global memory
  cudaMemcpy(dev_u, u, N*sizeof(cufftDoubleComplex), 
	     cudaMemcpyHostToDevice);

  // Create a FFT plan, this is similar to the FFTW plan discussed in
  //      the MPI section.
  // This plan is for a 1D double complex to double complex transform 
  //      (Z = double complex)
  cufftHandle plan;
  cufftPlan1d(&plan, N, CUFFT_Z2Z, 1);

  // Carry out the forward FFT.  Everything is done in device memory
  cufftExecZ2Z(plan, dev_u, dev_a, CUFFT_FORWARD);
	
  // The forward transform rescales the data so that its multiplied by 
  //      N/2, correct it with a rescaling kernel
  rescale<<<1, N>>>(dev_a, 2./N);

  // Wait for the stream to be completed before retrieving the data
  cudaDeviceSynchronize();

  // Retrieve the data from the device
  cudaMemcpy(a, dev_a, N*sizeof(cufftDoubleComplex), 
	     cudaMemcpyDeviceToHost);

  // print out the results
  for (i=0; i<N; ++i) {
    printf("%d: %5.1f + %5.1fi\n", i, a[i].x, a[i].y);
  }

  printf("-----\n");

  // Now do the inverse transform
  cufftExecZ2Z(plan, dev_a, dev_u, CUFFT_INVERSE);
  // The inverse transform does a different scaling
  rescale<<<1, N>>>(dev_u, 0.5);

  // Again wait for the stream to finish
  cudaDeviceSynchronize();

  // retrieve the results of the inverse transform
  cudaMemcpy(u, dev_u, N*sizeof(cufftDoubleComplex), 
	     cudaMemcpyDeviceToHost);

  // print the results
  for (i=0; i<N; ++i)
    printf("%d: %10.5f + %10.5fi == %10.5f\n", i, u[i].x, u[i].y,
	   sin(i*dx));

  // destroy the plan
  cufftDestroy(plan);
  // release the memory
  cudaFree(dev_u);
  cudaFree(dev_a);
  free(u);
  free(a);
  return 0;
}

// kernel for rescaling the results
__global__ void rescale(cufftDoubleComplex* dev_u, double f)
{
  dev_u[threadIdx.x].x = dev_u[threadIdx.x].x*f;
  dev_u[threadIdx.x].y = dev_u[threadIdx.x].y*f;
}
