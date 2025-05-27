# FFTW

The Fast Fourier Transform (FFT) has many applications, from filtering and image compression to solving partial differential equations. A basic approach for computing an FFT is to use the FFTW library. Here we provide an example `fft_example1D.c` that demonstrates the forward and inverse Fourier transform. 

1- Modify the code to do a 2D FFT transform.

2- Modify the code to do a multithreaded OpenMP 2D FFT transform.

3- Modify the code to do a FFT transform using the MPI version of the FFTW.

4- The standard CUDA also includes an FFT library, cuFFT. Write a code to use the library to do a 1D and 2D FFT.  

* Builiding code with FFTW

`
$ module load openmpi
`

`
$ module load fftw/3.3.8
`
  
`
$ gcc -o fft_example1D fft_example1D.c -lfftw3 -lm
`

`
$ gcc -o fft_example1D_omp fft_example1D_omp.c -fopenmp -lgomp -lfftw3_omp -lfftw3 -lm
`

`
$ mpicc -o fft_example1D_mpi fft_example1D_mpi.c -lfftw3_mpi -lfftw3 -lm
`

`
$ nvcc -o fft_example1D_cuda fft_example1D.cu -lcufft -lm
`

### HW Submission
Submit, through course Canvas, your codes. 
