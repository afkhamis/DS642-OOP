#include <CL/cl.h> // openCL headers (Khronos C Wrapper API)
#include <stdio.h>
#include <stdlib.h>


#define MAX_SOURCE_SIZE (0x100000)

int main(int argc, char ** argv) {

	int SIZE = 1024;

	// Allocate memories for input arrays and output array.
	float *A = (float*)malloc(sizeof(float)*SIZE);
	float *B = (float*)malloc(sizeof(float)*SIZE);

	// Output
	float *C = (float*)malloc(sizeof(float)*SIZE);
  
	// Initialize values for array members.
	for (int i = 0; i < SIZE; ++i) {
		A[i] = i;
		B[i] = i*2;
	}

	// Load kernel from file vecAddKernel.cl

	FILE *kernelFile;
	char *kernelSource;
	size_t kernelSize;

	kernelFile = fopen("vecAddKernel.cl", "r");

	if (!kernelFile) {
		fprintf(stderr, "No file named vecAddKernel.cl was found\n");
		exit(-1);
	}
  
	kernelSource = (char*)malloc(MAX_SOURCE_SIZE);
	kernelSize = fread(kernelSource, 1, MAX_SOURCE_SIZE, kernelFile);
	fclose(kernelFile);

	// Getting platform and device information
	cl_platform_id platformId = NULL; // OpenCL platform - request a device
	cl_device_id deviceID = NULL;     // Device ID
	cl_uint retNumDevices;           
	cl_uint retNumPlatforms;
	cl_int ret = clGetPlatformIDs(1, &platformId, &retNumPlatforms);   // Bind to platform
	ret = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, 1, &deviceID, &retNumDevices); // Get ID for the device

	// Creating context
	cl_context context = clCreateContext(NULL, 1, &deviceID, NULL, NULL,  &ret);

	// Creating command queue - sequence of commands sent to device
	cl_command_queue queue = clCreateCommandQueueWithProperties(context, deviceID, 0, &ret);

  // Create the input and output arrays in device memory for our calculation
	// Memory buffers for each array
	cl_mem d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(float), NULL, &ret);
	cl_mem d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(float), NULL, &ret);
	cl_mem d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, SIZE * sizeof(float), NULL, &ret);

	// Copy lists to memory buffers
	ret = clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0, SIZE * sizeof(float), A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(queue, d_b, CL_TRUE, 0, SIZE * sizeof(float), B, 0, NULL, NULL);

	// Create program from kernel source buffer
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, (const size_t *)&kernelSize, &ret);	

	// Build program executable (in OpenCL the kernel is compiled at runtime)
	ret = clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL);

	// Create the compute kernel (compile the kernel code)
	cl_kernel kernel = clCreateKernel(program, "addVectors", &ret);

	// Set arguments for kernel function
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&d_a);	
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&d_b);	
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&d_c);	

	// Execute the kernel
	size_t globalItemSize = SIZE; // Number of work items in each local work group
	size_t localItemSize = 64; // Number of total work items - globalItemSize has to be a multiple of localItemSize. 1024/64 = 16 
	ret = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalItemSize, &localItemSize, 0, NULL, NULL);	// Execute the kernel over the entire range of the data set

  // Wait for the command queue to get serviced before reading back results
  clFinish(queue);

	// Read from device back to host
	ret = clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0, SIZE * sizeof(float), C, 0, NULL, NULL);

	// Check the results
	for (i = 0; i < SIZE; ++i) 
		if (C[i] != (A[i] + B[i])) {
			printf("FAILED!. \n");
			break;
		}
	printf("PASSED! \n");
	

	// Clean up, free the memory.
	ret = clFlush(queue);
	ret = clFinish(queue);
	ret = clReleaseCommandQueue(queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(d_a);
	ret = clReleaseMemObject(d_b);
	ret = clReleaseMemObject(d_c);
	ret = clReleaseContext(context);
	free(A);
	free(B);
	free(C);

	return 0;
}
