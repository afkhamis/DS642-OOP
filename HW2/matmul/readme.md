# matmul

The goal of this homework is to perform some optimizations for matrix multiplication on Bridges-2 machine. In `membench` assignment, you study the memory hierachy and you should see from the results the latencies to different levels of the memory. This assignment is to optimize matrix multiplication by looking at loading and storing process in the cache hierarchy to increase performance. The operation is `C = C + A * B`. For simplicity, A, B, and C are square matrices. A naive C code is provided which evaluates the matrix multiply routine provided in `matmul.c`. The two column output gives the FLOPS rating for various matrix sizes. 

In this assignment, you will write your C code, so that the compiler can discover opportunities for optimization. You should also consider various sources of pipeline stall cycles. What you need to do is to replace the naive matrix multiply routine in `matmul.c` with your own routine. You can write your routine in C or Assembly (or both), in any number of source files. You can start using the naive code and choose compiler flags:

- -O3: Aggressive optimization

- -march=cpu-type: Tuning for a specific architecture (e.g. -march=corei7; you should figure out the appropriate architecture flag for the cluster yourself ). To find out what kind of CPU the system has, run the following command:

   `$ cat /proc/cpuinfo`

- -ftree-vectorize: Automatically use the SSE units intelligently (the Intel compiler is still much better at this).

- -funroll-loops: Unroll loops (basic loop unrolling is automatic with -O3).

- -ffast-math: Allow risky floating point optimizations.

Optimization:

What is provided:

|Implementation |Long description|
|:--------------|:---------------|
|dgemm_naive.c  |A trivial unoptimized implementation|
|dgemm_blocked.c|A simple blocked implementation of matrix multiply|
|dgemm_blas.c   |A wrapper which calls the vendor's optimized BLAS implementation of matrix multiply|
|matmul.c       |A timing harness and tester|

Note that "dgemm" stands for "**D**ouble Precision **GE**neral **M**atrix **M**ultiply". 

The necessary files are in [matmul.tar.gz](https://github.com/afkhamis/DS642-s24/blob/main/HW2/matmul/matmul.tar.gz)

You are supposed to perform blocking.  The dgemm-blocked.c already gets you started with this; you'll need to tune block sizes. For futher optimizations, think about loop unrolling and software pipelining.  The matrices are all stored in column-major order, i.e. C<sub>i,j</sub> `== C(i,j) == C[(i - 1) + (j - 1) * n]`, for `i = 1:n`, where `n` is the number of rows in `C`. 0-based indexing is used for C index notation `C[(i - 1) + (j - 1) * n]`.

## Submitting and running the code

You must use `module` to load the intel, gcc, openblas, etc., for example

`$ module load openblas/0.3.12-gcc10.2.0`

If you want you can play around with the Intel compiler, which may perform better than GCC on this type of code.

The jobs queue on Bridges is managed via the SLURM scheduler. To submit a job, use the `sbatch` command like so:

    sbatch submit-blocked.job

To check the status of your running jobs you can use the following command:

    squeue -u $USER

Append a `-l` flag will print additional information about the running jobs.
If you want even more information, consider using the `sacct` command, for example:

    sacct -j $JOBID --format JobID,ReqMem,MaxRSS,TotalCPU,State

where `$JOBID` is the ID number of the job.

If you want to cancel a job, run:

    scancel $JOBID

If you would like to receive emails for job submissions add the following lines to the submission scripts.
This sometimes helps tracking down issues.

    #SBATCH --mail-type=ALL
    #SBATCH --mail-user=youremailaddress

For more details on SLURM commands please see Bridges-2 documentation [https://www.psc.edu/resources/bridges-2/user-guide/].

There are three available benchmark modes: "naive", "blocked", and "blas". 
To compile and build all the executables, type:

    make

To benchmark the "blocked" mode, type:

    ./dgemm-blocked

To benchmark the "naive" mode, type:

    ./dgemm-naive

To benchmark the "blas" mode, type:

    ./dgemm-blas

The benchmark results will be written to a CSV file named `timing-MODE.csv` (replace `MODE` with the benchmarking mode).
The code outputs a sequence of timings to a CSV (tab-separated value) text file that can be processed with `gnuplot` for example. The name of the CSV file is based on the executable name. To run all the timers, type:

    make run

To clear up everything but the code, type:

    make clean

You can produce timing plots by running:

    make timing.pdf

which uses `gnuplot`, assuming that all the relevant CSV files are already in place.

### HW submission
The assignment is graded by reviewing your report, the optimization methods used, 
and benchmarking your code's performance. To benchmark your code, it will be compiled 
with the process detailed above, with the GNU compiler. A code that does not return 
correct results will receive significant penalties. Submit your code and report through course canvas. 

Your grade will depend on these factors: whether or not it completes running without exiting early, 
performance shown on the Brideges-2 supercomputer, your observation of the performance features 
you attempted. 






