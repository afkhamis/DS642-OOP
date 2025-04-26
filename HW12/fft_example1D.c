#include <stdio.h>
#include <math.h>
#include <fftw3.h>

/*
  int main(int argc, char* argv[])

  Compute the Fast Fourier Transform for data from sin(x)

  Inputs: none

  Outputs: Prints the Fourier coefficients and the data after
  both forward and backward transformation.
*/

int main(int argc, char* argv[])
{
#ifndef M_PI
  const double M_PI = 4.0*atan(1.0);
#endif
  int N = 16;
  fftw_complex *in, *out, *out2;
  fftw_plan p, pinv;
  in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  out2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  pinv = fftw_plan_dft_1d(N, out, out2, FFTW_BACKWARD, FFTW_ESTIMATE);
  for (int i=0; i<N; ++i) {
    double dx = 2.*M_PI/N;
    in[i][0] = sin(i*dx);
    in[i][1] = 0.;
  }
  fftw_execute(p);
  fftw_execute(pinv);
  for (int i=0; i<N; ++i)
    printf("a[%d]: %f + %fi\n", (i<=N/2 ? i : i-N), out[i][0]/N, 
	   out[i][1]/N);
  printf("- - -\n");
  for (int i=0; i<N; ++i)
    printf("f[%d]: %f + %fi == %f + %fi\n", i, out2[i][0]/N, 
	   out2[i][1]/N, in[i][0], in[i][1]);

  fftw_destroy_plan(p);
  fftw_destroy_plan(pinv);
  fftw_free(in);
  fftw_free(out);
  fftw_free(out2);
}
