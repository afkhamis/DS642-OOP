#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Get_dims(int* m_p, int* n_p);
void Mat_vect_mult(double A[], double x[], double y[], int m, int n);

/*-------------------------------------------------------------------*/
int main(void) {
   double* A , * x, * y;
   int m, n, i, j;

   Get_dims(&m, &n);
   A = malloc(m*n*sizeof(double));
   x = malloc(n*sizeof(double));
   y = malloc(m*sizeof(double));
   if (A == NULL || x == NULL || y == NULL) {
      fprintf(stderr, "Can't allocate storage\n");
      exit(-1);
   }

   srandom(time(NULL));
   
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++) 
         A[i*n + j] = ((double) random())/((double) RAND_MAX);

   for (i = 0; i < n; i++)
      x[i] = ((double) random())/((double) RAND_MAX);
   
   Mat_vect_mult(A, x, y, m, n);

   free(A);
   free(x);
   free(y);
   return 0;
}  

void Get_dims(
              int*  m_p  /* out */, 
              int*  n_p  /* out */) {
   printf("Enter the number of rows\n");
   scanf("%d", m_p);
   printf("Enter the number of columns\n");
   scanf("%d", n_p);

   if (*m_p <= 0 || *n_p <= 0) {
      fprintf(stderr, "m and n must be positive\n");
      exit(-1);
   }
}  

void Mat_vect_mult(
                   double  A[]  /* in  */, 
                   double  x[]  /* in  */, 
                   double  y[]  /* out */,
                   int     m    /* in  */, 
                   int     n    /* in  */) {
   int i, j;

   for (i = 0; i < m; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
         y[i] += A[i*n+j]*x[j];
   }
}  
