# Homework 6

In these exercises, we will write simple cuda programs. You will learn how to allocate/free GPU memory (devise), move/copy data between/from the CPU (host) and the GPU, and launch kernels. Pay close attention to the kernel call parameters, block and grid sizes! Can you write the kernel so that it functions even if you launch too many threads?

## **1. Hello World**

The code skeleton is provided in `hello.cu`. Edit the file, paying close attention to the kernel call parameters (noted by FIXME in the code), so that the output when run is like this:

```
Hello from block: 0, thread: 0
Hello from block: 0, thread: 1
Hello from block: 1, thread: 0
Hello from block: 1, thread: 1
```

(the ordering of the above lines may vary; ordering differences do not indicate an incorrect result)

Note the use of `cudaDeviceSynchronize()` after the kernel launch. In CUDA, kernel launches are *asynchronous* to the host thread. The host thread will launch a kernel but not wait for it to finish, before proceeding with the next line of host code. Therefore, to prevent application termination before the kernel gets to print out its message, we must use this synchronization function.

After editing the code, compile it using the following:

```
module load cuda
nvcc -o hello hello.cu
```

The module load command selects a CUDA compiler for your use. The module load command only needs to be done once per session/login. `nvcc` is the CUDA compiler invocation command. The syntax is generally similar to gcc/g++.

To run your code on Bridges2, use `hello.submit` script:

```
$ sbatch hello.submit 
```

You can also run in an interactive session, see https://www.psc.edu/resources/bridges-2/user-guide/ for further information.

## **2. Computing $\pi$**

A CUDA program that uses numerical integration to estimate $\pi$ is given, `pi.cu`. Compile it and run it similar to the method given in exercise 1. Add CUDA codes to it to measure the performance in CUDA runtime. The tools you can use to measure the time GPU spends on a task is the CUDA Event API (https://docs.nvidia.com/cuda/).

## **3. Vector Add**

Write a complete vector add program from scratch. If you need help, a skeleton of the code is given in `vector_add.cu`, which only uses one thread on the GPU card. You can extend it to do the vector addition with blocks and blocks and threads.


## **Running Your Code **

To run your code at on Bridges-2, we will use SLURM:

```
sbatch job.submit
```

where the `job.submit` script contains the commands in order run the code on GPU nodes.

Allocation is on Bridges-2 only for this course, and should not be used for any other purposes.

To reserve a GPU in an interactive session, see the Bridges-2 manual: https://www.psc.edu/resources/bridges-2/user-guide/

Please also note jobs in the GPU-shared partition use only part of one node. Because SUs are calculated using how many gpus are used, using only part of a node will result in a smaller SU charge. Use your discretion when running on GPU nodes to minimize the SU charge, due to limited availability. 


### HW submission
Submit, through the course canvas, your `hello.cu`, `pi.cu` and `vector_add.cu`.
    
  




