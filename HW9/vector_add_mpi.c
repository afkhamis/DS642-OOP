#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MASTER 0
#define N 10000000
#define MAX_ERR 1e-6

void vector_add(double *c, double *a, double *b, int n) {

    for(int i = 0; i < n; i ++)
        c[i] = a[i] + b[i];
}

int main (int argc, char *argv[]) {
	double *a, *b, *c;
  int i, total_proc, my_rank, local_n;
	int n = N;

	MPI_Status status;
  MPI_Comm comm;

	MPI_Init (&argc, &argv);
  comm = MPI_COMM_WORLD;
	MPI_Comm_size (comm, &total_proc);
	MPI_Comm_rank (comm, &my_rank);

  double * local_a, *local_b, *local_c;

	if (my_rank == MASTER)  {
		a = (double *) malloc(sizeof(double)*n);
		b = (double *) malloc(sizeof(double)*n);
		c = (double *) malloc(sizeof(double)*n);

		for(i = 0; i < n; i++) a[i] = 1.0;
		for(i = 0; i < n; i++) b[i] = 2.0;
	}

	local_n = n/total_proc;

	local_a = (double *) malloc(sizeof(double)*local_n);
	local_b = (double *) malloc(sizeof(double)*local_n);
	local_c = (double *) malloc(sizeof(double)*local_n);

	MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, MASTER, comm);
	MPI_Scatter(b, local_n, MPI_DOUBLE, local_b, local_n, MPI_DOUBLE, MASTER, comm);

  vector_add(local_c, local_a, local_b, local_n);

	MPI_Gather(local_c, local_n, MPI_DOUBLE, c, local_n, MPI_DOUBLE, MASTER, comm);

	if (my_rank == MASTER) {
		int passed = 1;
		for(i = 0;i < n; i++)
			if (fabs(c[i]- a[i] - b[i]) > MAX_ERR) {
				printf("problem at index %lld\n", i);
				passed = 0;
				break;
			}
		if (passed) printf ("PASSED!\n");
	}

	if (my_rank == MASTER)  {
		free(a);  
    free(b); 
    free(c);
	}
	free(local_a);  
  free(local_b); 
  free(local_c);

	MPI_Finalize();

	return 0;
}
