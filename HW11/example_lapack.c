#include <stdio.h>
#include <lapacke.h>  // The C interface for LAPACK

/*
  int main(int argc, char* argv[])

  The main program creates a statically allocated linear system, 
  and then solves Ax = b.  
  The original problem with the solution is printed out at the end.

  Inputs: none

  Outputs: Prints the original problem with the solution
*/

int main(int argc, char* argv[])
{
  // The dimension of the matrix A
  // lapack_int is a special type defined for LAPACK, 
  //      it is usually the same as int
  lapack_int n = 5;
	
  // The number of columns of b.  nrhs can be >= 1 if multiple solutions
  //      for the same A are desired
  lapack_int nrhs = 1;
	
  // The matrix A stored in column-major order
  double a[5][5] = {{-2.,2.,0.,0.,0.},
		    {1.,-2.,1.,0.,0.},
		    {0.,1.,-2.,1.,0.},
		    {0.,0.,1.,-2.,1.},
		    {0.,0.,0.,1.,-2.}};
	
  // The solver will not preserve the matrix, so we will keep a copy of
  //      the original A so that we can print it out at the end.
  double aorig[5][5];
  for (int i=0; i<5; ++i)
    for (int j=0; j<5; ++j)
      aorig[i][j] = a[i][j];
			
  // The function asks for a pointer to the matrix array, so we have to
  //      create a pointer of the right type.  
  double* A = &(a[0][0]);
	
  // This is the right hand side, also stored in column-major format.
  double b[5] = {1.,2.,3.,4.,5.};
	
  // Upon return, the answer will be stored in b, so we have to keep
  //      a copy of b so we can print the original problem at the end.
  double borig[5];
  for (int i=0; i<5; ++i)
    borig[i] = b[i];
		
  // Variable lda is the leading dimension of the matrix A, 
  //      generally the same as n above.
  lapack_int lda = 5;
	
  // Variable ipiv is an array for storing the row order.  
  // If there are row interchanges during Gaussian elimination, that
  //      info is stored in ipiv.  
  // The data will be initialized within LAPACK, so we only have to 
  //      create it, and it has to have storage for as many integers 
  //      as the rows in A.
  lapack_int ipiv[5];
	
  // This is the leading dimension of the right hand side, generally
  //      will be the same as lda.
  lapack_int ldb = 5;
	
  // If there is an error, a code indicating the type of error will be
  //      stored in info upon return.
  lapack_int info = 0;

  // This is the actual LAPACK call.  
  // Note that all variables are passed by reference.
  // Upon return A and ipiv will have the encoded LU decomposition, 
  // and b will contain the solution to the original linear system.
  info = LAPACKE_dgesv(LAPACK_COL_MAJOR, n, nrhs, A, lda, ipiv, b, ldb);

  // Print out the original system Ax = b replacing x with the solution.
  printf("Check this:\n");
  for (int i=0; i<5; ++i) {
    printf("[");
    for (int j=0; j<5; ++j) {
      // Note that the aorig is printed out as if a transpose, 
      // because that's the way Fortran and LAPACK interpret the array 
      // (different from C).
      printf("%9.3f", aorig[j][i]);
    }
    printf("] [%9.3f]", b[i]);
    if (i==2)
      printf(" = ");
    else
      printf("   ");
    printf("[%5f]\n", borig[i]);
  }
  return 0.;
}
