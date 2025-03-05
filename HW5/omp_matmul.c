#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 8 /* Matrices size */

int main(int argc, char *argv[])
{
  int i, j, k, n = N, tid, nthreads, chunk = 1;

  double * AA = malloc((n * n) * sizeof(double));
  double * BB = malloc((n * n) * sizeof(double));
  double * CC = malloc((n * n) * sizeof(double));

/*
	for (int i = 0; i < n * n; i++)
		AA[i] = i + 1;

	// Accessing the array values as if it was a 2D array 
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			printf("%6.2f ", AA[i * n + j]);
		printf("\n");
	}
*/

#define A(i,j) AA[i+j*n]
#define B(i,j) BB[i+j*n]
#define C(i,j) CC[i+j*n]

/*** Spawn a parallel region explicitly scoping all variables ***/
#pragma omp parallel shared(AA,BB,CC,nthreads,chunk) private(tid,i,j,k)
{
  tid = omp_get_thread_num();
  if (tid == 0){
    nthreads = omp_get_num_threads();
    printf("Starting matrix multiple example with %d threads\n", nthreads);
    printf("Initializing matrices...\n");
  }

/* Initialize A, B, and C matrices */
#pragma omp for
    for (i = 0; i < n; i++)
       for (j = 0; j < n; j++)
          A(i,j) = i+j;
#pragma omp for
    for (i = 0; i < n; i++)
       for (j = 0; j < n; j++)
          B(i,j)= i*j;
#pragma omp for
    for (i = 0; i < n; i++)
       for (j = 0; j < n; j++)
          C(i,j) = 0.;

/*** Do matrix multiply sharing iterations on outer loop ***/
/*** Display who does which iterations for demonstration purposes ***/
  printf("Thread %d starting matrix multiply...\n",tid);
#pragma omp for 
    for (i = 0; i < n; ++i) {
      printf("Thread=%d did row = %d\n",tid,i);
      for (j = 0; j < n; ++j) {
        for (k = 0; k < n; ++k)
          C(i,j) += A(i,k)*B(k,j);
      }
    }
}     /*** End of parallel region ***/

    printf("Here is the result matrix:");
    for (i = 0; i < n; i++) {
      printf("\n");
      for (j = 0; j < n; j++)
        printf("%6.2f   ", C(i,j));
    }

  printf ("\nDone.\n");

  free(AA);
  free(BB);
  free(CC);

  return 0;
}
