# Project: Implementing and Optimizing a Distributed Preconditioned Conjugate Gradient (CG) Solver


## 

In this project, you will develop a distributed version of the preconditioned conjugate gradient method for solving linear systems; see for example:

[https://en.wikipedia.org/wiki/Conjugate_gradient_method
](https://en.wikipedia.org/wiki/Conjugate_gradient_method)
## 

Instructions:

*   The goal of this assignment is to use preconditioned conjugate gradients (PCG) to solve $Ax = b$ (obtained from the finite difference discretization of the 1D Poisson's equation), where $A$ is $N \times N$, which has 2s on its diagonal and -1s on its off-diagonals (above and below the diagonal). The vector b is all 1s, and the PCG method starts with an initial guess $x$ of all 0s. The starter code only works for 1 rank, and it is not efficient. In this project, you will use MPI to develop a distributed version of this PCG solver. For simplicity, we set N to be a multiple of the number of ranks used. You must do the following:
    
    1.  Sparse Matrix Format Conversion: Translate the provided starter-code–map-based sparse matrix representation—to a more efficient format (i.e. [CSR (Compressed Sparse Row) or CSC (Compressed Sparse Column)](https://en.wikipedia.org/wiki/Sparse_matrix))
        
    2.  Layout Formulation: Figure out how to distribute the sparse matrix format across MPI processes.
        
    3.  Parallelize matrix-vector multiplication and norm across different MPI ranks.
        
    4.  Optimize the PCG framework: Is there any opportunity to reuse intermediate values or reorder operations to make the solver faster? Do the methods you used affect the residual error, and why?
        
    5.  Scaling: Evaluate strong and weak scaling of your distributed PCG solver by conducting tests with varying numbers of ranks to understand the performance characteristics of your implementation. Evaluate speedups across 1-64 ranks for one node and across different $N \times N$ matrices (N=2^20 - 2^26).
        
        *   Plot the relationship between the number of PCG iterations and the number of ranks used, for a few N. Explain the behavior based on your understanding of the algorithm and the code.
            
        *   Plot the relationship between the residual error and the number of ranks used, for a few N. Explain the behavior based on your understanding of the algorithm and the code.
            
### 

Submission Requirements

Your submission should include distributed\_pcg.cpp and the report pdf. The report must detail your implementation strategy, performance comparisons, scalability results, and any insights or challenges you encountered throughout the assignment. Ensure your code is well-documented and follows the assignment specifications closely.

## 

Starter Code

The starter code is available on GitHub at [https://github.com/Berkeley-CS267/HW4  
](https://github.com/Berkeley-CS267/HW4)

Note that this starter code on GitHub only runs on 1 node and 1 task.   

Below is how to build your code: 

## Building the Code

1. **Load CMake:**
    ```sh
    module load cmake
    ```

2.  **Create a build directory:**
    ```sh
    mkdir build
    cd build
    ```

3.  **Configure the project using CMake:**
    ```sh
    cmake -DCMAKE_BUILD_TYPE=Release ..
    ```

4.  **Compile the code:**
    ```sh
    make
    ```
    This will create an executable named `pcg` in the build directory.

## The problem
The goal is to solve $Ax = b$ where $A = L$ is an $N \times N$ s.p.d. matrix with $L$ representing the discretized 1D Poisson's equation. The right hand side $b$ is all 1s and the preconditioned conjugate gradient starts with an initial guess $x$ of all 0s. You can only modify the `distributed_pcg.cpp` file.

## Running the Code

```sh
./pcg -N <size of the matrix>
```
This runs the code using only one task and it is not efficient. For the scaling study, you must change the number of tasks and the size of the matrix accordingly.

