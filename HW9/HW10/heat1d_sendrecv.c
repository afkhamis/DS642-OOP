#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>  // This is the header file for MPI functions

/*
  int main(int argc, char* argv[])

  Heat flow solver.  All data is initially zero, boundary conditions are
  provided as input

  Inputs: argc should be 2
  argv[1]: number of grid points
  User prompted for left and right boundary conditions

  Outputs: Prints out the final values.

*/

int main(int argc, char* argv[])
{
  // First thing is to initialize the MPI interface.  
  // Some arguments can be passed through to the MPI interface, 
  // so the argument list is sent by sending the argument list
  MPI_Init(&argc, &argv); 

  int N = atoi(argv[1]); // Get the number of grid points

  // Determine the rank of the current process
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Determine the number of processes
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // allocate memory.  Each core should have roughly N/size + 2 grid
  // points, but if N is not evenly divisible by size, give one extra
  // grid point to the low ranks to make up the difference.  Also, the
  // rank=0 and rank=size-1 processes do not have to store boundary
  // data for a neighbor.
  int localN = N/size + (N%size > rank ? 1 : 0) + (rank > 0 ? 1 : 0) 
    + (rank < size-1 ? 1 : 0);
  double* u[2];
  // need two copies to do updates
  u[0] = (double*)malloc(localN * sizeof(double));  
  u[1] = (double*)malloc(localN * sizeof(double));
	
  // Initialize the data
  for (int i=0; i<localN; ++i)
    u[0][i] = 0.;

  if (rank == 0) {
    // Read the boundary conditions from the input list.
    // Get the left boundary condition.
    u[0][0] = atof(argv[2]); 
    u[1][0] = u[0][0];
  }

  if (rank == size-1) {
    // Get the right boundary condition
    u[0][localN-1] = atof(argv[3]);
    u[1][localN-1] = u[0][localN-1];
  }

  // CFL condition: dt < 0.5 dx^2
  double dx = 1./(N-1);
  double dx2 = dx*dx;
  double dt = 0.25 * dx2;

  // Storage for tracking communicaton info
  MPI_Status status;

  // main loop: terminal time is T=1
  int i, newi, oldi;
  for (i=0; i*dt < 1.0; ++i) {
    newi = (i+1)%2;
    oldi = i%2;
    // Exchange end data 
    if (rank > 0 && rank < size-1) {
      // Move data to the left 
      MPI_Sendrecv(&(u[oldi][1]), 1, MPI_DOUBLE, rank-1, i, 
		   &(u[oldi][localN-1]), 1, MPI_DOUBLE, rank+1, i, 
		   MPI_COMM_WORLD, &status);
      // Move data to the right
      MPI_Sendrecv(&(u[oldi][localN-2]), 1, MPI_DOUBLE, rank+1, i,
		   &(u[oldi][0]), 1, MPI_DOUBLE, rank-1, i, 
		   MPI_COMM_WORLD, &status);
    }
    // Handle end cases
    if (rank == 0 && size > 0) {
      // if data moves left, then rank 0 receives from the right, 
      //      but does not send left
      MPI_Recv(&(u[oldi][localN-1]), 1, MPI_DOUBLE, 1, i, 
	       MPI_COMM_WORLD, &status);
      // if data moves right, then rank 0 sends to the right, 
      //      but does not receive from left
      MPI_Send(&(u[oldi][localN-2]), 1, MPI_DOUBLE, 1, i, 
	       MPI_COMM_WORLD);
    }
    if (rank == size-1 && size > 0) {
      // if data moves left, then rank size-1 sends to the left
      MPI_Send(&(u[oldi][1]), 1, MPI_DOUBLE, rank-1, i, MPI_COMM_WORLD);
      // if data moves right, then rank size-1 receives from the left
      MPI_Recv(&(u[oldi][0]), 1, MPI_DOUBLE, rank-1, i, MPI_COMM_WORLD, 
	       &status);
    }		
    // Now do the update 
    for (int j=1; j<localN-1; ++j) 
      u[newi][j] = u[oldi][j]+dt*(u[oldi][j+1] - 2*u[oldi][j] 
				  + u[oldi][j-1])/dx2;				
  }

  // We have to gather all the data from the various
  //      processes so it can be printed. 
  // We'll send all the data to rank==0.  
  // We'll see a better way to do this later.
  if (rank == 0) {
    // allocate space for the full array
    double* finalu = (double*)malloc(N * sizeof(double));

    // copy the local data to the full array
    for (int j=0; j<localN-1; ++j)
      finalu[j] = u[newi][j];

    // Track where the next array data will be inserted in the
    //      full array
    int nextj = localN-1;
    int datalen;

    // request the data from each of the other processes,
    //      appending as we go.
    for (int r=1; r<size; ++r) {
      // amount of data in rank=r process excluding internal boundary
      //      data
      datalen = N/size + (N%size > r ? 1 : 0); 
      MPI_Recv(&(finalu[nextj]), datalen, MPI_DOUBLE, r, 0,
	       MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      nextj += datalen; // update the new insertion point
    }

    // Store the final array in binary format to file finalu.dat
    FILE* fileid = fopen("finalu.dat", "w");
    fwrite(finalu, sizeof(double), N, fileid);
    fclose(fileid);

    // Release the finalu allocation of memory
    free(finalu);

  } else {
    // All ranks other than 0 must send their interior data to rank 0
    // The last rank doesn't have a boundary point,
    //      so it's one bigger than the others.
    if (rank < size-1) 
      MPI_Send(&(u[oldi][1]), localN-2, MPI_DOUBLE, 0, 0,
	       MPI_COMM_WORLD);
    else
      MPI_Send(&(u[oldi][1]), localN-1, MPI_DOUBLE, 0, 0,
	       MPI_COMM_WORLD);
  }

  free(u[0]);
  free(u[1]);

  // Must shut down the MPI system when you're done.
  // If you don't, there can be lots of junk left behind.
  MPI_Finalize();

  return 0;
}
