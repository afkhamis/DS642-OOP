# Review / Compile / Run OpenMP example code

Compute $\pi$ by numerical integration, using Riemann sums. We know

$$ \int_0^1 {4 \over 1 + x^2} dx= \pi$$

We can approximate the integral as a sum of rectangle:

$$\sum_{i=0}^n {4 \over 1 + x_i^2} \Delta x $$

where $x_i = i \Delta x$ and $\Delta x = 1/n$.

A serial program for this is provided `pi_serial.c`; your task is to parallelize it using OpenMP. You are supposed to examine various implementations and suggest the one that is most optimized. You can start with parallel for directives; each thread calculates a value based on its index, and these are then centrally summed. You can also compare this with when using a loop and a reduction. 


Next, use different numbers of cores and compute the speedup you attain over the sequential computation. Is there a performance difference between the OpenMP codes when increasing the number of threads? Is the scaling as expected - suppose you have a code that runs in time $T = O(n)$ on a single processor. Then you would hope to run close to time $T/p$ when using $p$ processors - will you reach this speedup using OpenMP? Can an efficient implementation reach a completely linear speedup? Calculate how many floating-point operations per step needed for summing the series above (ignoring the overhead for iterating the loop), and then provide an estimate of the computational speed by measuring the time taken to sum it.  


For performance assessment, there will be two types of scaling that are tested for a parallel code:

  *  In strong scaling we keep the problem size constant but increase the number of processors

  *  In weak scaling we increase the problem size proportionally to the number of processors so the work/processor stays the same

### HW submission
Submit a report for the followings:

1- A description of your OpenMP implementation/s and how your serial and parallel codes scale with the number of rectangles.

2- Speedup plot/s and a discussion on whether or not your OpenMP code approach the ideal speedup.


