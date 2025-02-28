#include <stdlib.h>          /* Standard Library */
#include <stdio.h>           /* IO library */
#include <math.h>            /* Math librry */
#define pi_true (acos(-1.))  /* double pi_true = 3.14159265358979323846; */

int main(int argc, char* argv[]) {

  double h, x, pi=0.;
  int i;
  long int n = 100000;

  h = 1. / (double) n; /* Set subinterval size */

  for (i = 0; i < n; i++) {   /* Perform integration */
    x = h * (i + .5);
    pi += 4.*h/(1. + x*x);
  }

  printf(" computed pi = %.16e error = %.16e\n",
           pi, pi_true - pi);

  return(0);
}
