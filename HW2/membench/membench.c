/* Based on:
 *
 *   https://people.eecs.berkeley.edu/~demmel/cs267_Spr99/Assignments/membench/membench.c
 *
 * This version uses the OpenMP omp_get_wtime() routine.
 */

 #include <stdio.h>
 #include <omp.h>

 #define SAMPLE    10
 #define CACHE_MIN (1024)
 #define CACHE_MAX (16*1024*1024)
 #define RTIME     0.1

 int x[CACHE_MAX];

 int main(int argc, char** argv) {

   int i, index, stride, limit, temp;
   long steps, tsteps, csize;
   double sample_ns, sample_sec, sec, sec0, sec1,
          start, ns_per_step, reads_per_step;


/* Run the timing experiments */
   for (csize = CACHE_MIN; csize <= CACHE_MAX; csize *= 2){
       for (stride = 1; stride <= csize/2; stride *= 2){
          sec0 = 0;
          sec1 = 0;
          limit = csize - stride + 1;

           /* Time the loop with strided access + loop overhead */
          steps = 0;
          start = omp_get_wtime();
          do {
              for (i = SAMPLE*stride; i != 0; i--)
                  for (index = 0; index < limit; index += stride)
                      x[index]++;
              steps++;
              sec0 = omp_get_wtime() - start;
          } while (sec0 < RTIME);

           /* Try to time just the overheads */
          tsteps = 0;
          temp = 0;
          start = omp_get_wtime();
          do {
              for (i = SAMPLE*stride; i != 0; i--)
                  for (index = 0; index < limit; index += stride)
                      temp += index;
              tsteps++;
              sec1 = omp_get_wtime() - start;
          } while (tsteps < steps);

           /* Report on the average time per read/write */
           sec            = sec0 - sec1;
           ns_per_step    = (sec*1.0e9)/steps;
           reads_per_step = SAMPLE*stride*((limit - 1.0)/stride + 1.0);
           printf("Size: %7d Stride: %7d read+write: %14.2f ns\n",
                  csize*sizeof(int),
                  stride*sizeof(int),
                  ns_per_step/reads_per_step);
                  fflush(stdout);
       }
       printf ("\n");
   }
   return 0;
 }

