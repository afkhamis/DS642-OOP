// OpenCL kernel. Each work item takes care of one element of c

__kernel void addVectors(__global const float *a,
                         __global const float *b,
                         __global float *c) {

        // Get our global thread ID
        int id = get_global_id(0);

        c[id] = a[id] + b[id];
}  // Execute over n work-items
