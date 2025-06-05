# Module 6: Assignment-Matrix Multiplication with Open MP

OpenMP parallelization of matrix multiplication program `omp_matmul.c`.

A working code is provided that performs a matrix multiplication by distributing the iterations of the operation between available threads.
After reviewing the source code, compile and run the program. Review the output. It shows which thread did each iteration and the final result matrix.

Vary the size of your matrices from 250, 500, 750, 1000, and 2000 and measure the runtime with one thread. For each matrix size, change the number of threads from
2 to 8, and plot the speedup versus the number of threads. Compute the efficiency.

Run the program again, however this time sort the output to clearly see which threads execute which iterations.


  *  Can you optimize the program by playing with how the loops are scheduled?
  *  Try – Cache blocking – Loop unrolling – Vectorization and see how they interact with the constructs in OpenMP.
  *  Can you approach the peak performance of the computer?

### HW submission
Submit your report through the course canvas.
    
  
