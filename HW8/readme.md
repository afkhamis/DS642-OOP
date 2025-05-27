# OpenCL Vector Addition
This exercise shows a minimal conversion from our CUDA vector addition code to an OpenCL version. Inspect and verify that you can run an OpenCL kernel. Look at the host code and identify the API calls in the host code. After compiling and running, a message verifying that the program completed successfully is printed out. 

**Compiling**:

`
module load cuda
`

`
$ nvcc vector_add.c -o vector_add -lm -lOpenCL -arch=compute_70 -code=sm_70
`

The goal of this exercise is to verify that you understand manipulating kernel invocations and buffers in OpenCL. Add additional buffer objects and assign them to vectors defined on the host. Chain the vector add kernels ... e.g. C=A+B; D=C+E; F=D+G. Read back the final result and verify that it is correct. As you modify the code to chain vector add kernels, you’ll need to create additional buffers enqueue additional kernels. Next, modify the kernel so it adds three vectors together. Modify the host code to define three vectors and associate them with relevant kernel arguments. Read back the final result and verify that it is correct.

### HW submission
Submit, through the course canvas, your codes.
    

