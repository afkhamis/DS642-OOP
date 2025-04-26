#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <fftw3-mpi.h>

/*
  int main(int argc, char* argv[])

  Do a forward and reverse Fourier transform on a 2D array
  where the array is distributed.

  Inputs: 
  argv[1]: Length of the first dimension
  argv[2]: Length of the second dimension

  Outputs: Prints out the final values.

*/

int main(int argc, char* argv[])
{
  // Initialize MPI
  MPI_Init(&argc, &argv);
	
  // Initialize FFTW for MPI
  fftw_mpi_init(); 
	
  // Get dimensions of the domain
  const ptrdiff_t M = atoi(argv[1]); 
  const ptrdiff_t N = atoi(argv[2]);
  ptrdiff_t localM, local0;

  // Determine the amount of local memory required
  ptrdiff_t alloc_local = fftw_mpi_local_size_2d(M, N, MPI_COMM_WORLD, 
						 &localM, &local0);
						 
  // Allocate the local memory
  fftw_complex* datain = fftw_alloc_complex(alloc_local);
  fftw_complex* dataout = fftw_alloc_complex(alloc_local);

  // Set up the transform plans
  fftw_plan pf, pb;
  pf = fftw_mpi_plan_dft_2d(M, N, datain, dataout, MPI_COMM_WORLD, 
			    FFTW_FORWARD, FFTW_ESTIMATE);
  pb = fftw_mpi_plan_dft_2d(M, N, dataout, dataout, MPI_COMM_WORLD, 
			    FFTW_BACKWARD, FFTW_ESTIMATE);

#ifndef M_PI
  const double M_PI = 4.0*atan(1.0);
#endif

  // Set up the initial data on the local allocation
  double dx = 2*M_PI/M;
  double dy = 2*M_PI/N;
  for (int i=0; i<localM; ++i) {
    double x = dx*(local0+i); 
    for (int j=0; j<N; ++j) {
      double y = dy*j;
      datain[i*N+j][0] = cos(2*x+y);
      datain[i*N+j][1] = sin(2*x+y);
    }
  }
	
  // Execute the forward transform
  fftw_execute(pf); 

  // Set up rank 0 to receive the full array upon completion
  double* fulldatain = NULL;
  double* fulldataout = NULL;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    fulldatain = (double*)malloc(2*M*N*sizeof(double)); 
    fulldataout = (double*)malloc(2*M*N*sizeof(double));
  }

  // Assemble the results into the full array on rank 0 process
  MPI_Gather(dataout, 2*localM*N, MPI_DOUBLE, fulldataout, 2*localM*N, 
	     MPI_DOUBLE, 0, MPI_COMM_WORLD); 

  // Print the results 
  if (rank == 0) {
    for (int i=0; i<M; ++i) {
      for (int j=0; j<N; ++j) {
	double real = fulldataout[i*2*N+2*j]; 
	double imag = fulldataout[i*2*N+2*j+1];
	printf("%4.2f%c%4.2fi\t", real/M/N, 
	       imag >= 0 ? '+' : '-', fabs(imag/M/N));
      }
      printf("\n");
    }
  }

  // Execute the backward transform
  fftw_execute(pb); 

  MPI_Gather(dataout, 2*localM*N, MPI_DOUBLE, fulldataout, 2*localM*N, 
	     MPI_DOUBLE, 0, MPI_COMM_WORLD); 

  fftw_destroy_plan(pf);
  fftw_destroy_plan(pb);
  fftw_free(datain);
  fftw_free(dataout);
  if (rank == 0) {
    free(fulldatain);
    free(fulldataout);
  }
  MPI_Finalize();
}
