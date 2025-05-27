# Data distributions with MPI 

1- In this exercise, we want to write a vector addition program in MPI. The work consists of adding the individual components of the vectors, so there is no communication between the tasks. You can assume that the number of components evenly divides between the number of processors. To begin with, you can simply assign blocks of the vector consecutive components to each process; after that, each process simply adds its assigned components. The serial version of the code is given `vector_add.c`

* Compiling and running MPI code:

  ```
  Load MPI:  $ module load openmpi  
  Compile:  $ mpicc -g -Wall -o vector_add_mpi vector_add_mpi.c
  Test (on login node):  $ mpiexec -n <# of processors> ./vector_add
  ```

2- In this exercise, you will measure the communication cost, using a ping-pong communication, see the pseudocode below:

```
Process 0:
for i = 1:ntrials
  send b bytes to 1
  recv b bytes from 1
end

Process 1:
for i = 1:ntrials
  recv b bytes from 0
  send b bytes to 0
end
```

In order to measure the compute time, use `MPI_Wtime`, and insert it before and after the ping and then plot the compute time per message as a function of the message size. The skeleton of send and receive is given to you in `send_receive.c`.

3- A code, using messages and point-to-point communications, `MPI_Send()` and `MPI_Recv()`, is procvided for approximating $\pi$ using numerical integration `pi_mpi.c`. This implementation uses rank 0 processor for computing the final result, which becomes necessarily a communication bottleneck if the number of messages gets large. Your homework is to implement a more optimized method: a scatter-gather that can distribute terms in the sum to multiple processors and then collect the result using `MPI_reduce()`, and then compare the execution speeds.


### HW submission
Submit, through course Canvas, your codes and output of your codes.  

