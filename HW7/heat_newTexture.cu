
/*
 * Copyright 1993-2010 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and 
 * proprietary rights in and to this software and related documentation. 
 * Any use, reproduction, disclosure, or distribution of this software 
 * and related documentation without an express license agreement from
 * NVIDIA Corporation is strictly prohibited.
 *
 * Please refer to the applicable NVIDIA end user license agreement (EULA) 
 * associated with this source code for terms and conditions that govern 
 * your use of this NVIDIA software.
 * 
 */

#include "cuda.h"
#include "book.h"
#include "cpu_anim.h"

#define DIM 1024
#define PI 3.1415926535897932f
#define MAX_TEMP 1.0f
#define MIN_TEMP 0.0001f
#define SPEED 0.25f

// Updated to use cudaTextureObject_t
cudaTextureObject_t texConstSrcObj, texInObj, texOutObj;
// these exist on the GPU side
//texture<float>  texConstSrc;
//texture<float>  texIn;
//texture<float>  texOut;



// this kernel takes in a 2-d array of floats
// it updates the value-of-interest by a scaled value based
// on itself and its nearest neighbors
__global__ void blend_kernel(float *dst, bool dstOut, cudaTextureObject_t texIn, cudaTextureObject_t texOut) {
    // map from threadIdx/BlockIdx to pixel position

    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    int offset = x + y * blockDim.x * gridDim.x;

    int left = offset - 1;
    int right = offset + 1;
    if (x == 0) left++;
    if (x == DIM - 1) right--;

    int top = offset - DIM;
    int bottom = offset + DIM;
    if (y == 0) top += DIM;
    if (y == DIM - 1) bottom -= DIM;

    float t, l, c, r, b;
    if (dstOut) {
        t = tex1Dfetch<float>(texIn, top);
        l = tex1Dfetch<float>(texIn, left);
        c = tex1Dfetch<float>(texIn, offset);
        r = tex1Dfetch<float>(texIn, right);
        b = tex1Dfetch<float>(texIn, bottom);
    } else {
        t = tex1Dfetch<float>(texOut, top);
        l = tex1Dfetch<float>(texOut, left);
        c = tex1Dfetch<float>(texOut, offset);
        r = tex1Dfetch<float>(texOut, right);
        b = tex1Dfetch<float>(texOut, bottom);
    }
    dst[offset] = c + SPEED * (t + b + r + l - 4 * c);
}
// NOTE - texOffsetConstSrc could either be passed as a
// parameter to this function, or passed in __constant__ memory
// if we declared it as a global above, it would be
// a parameter here: 
// __global__ void copy_const_kernel( float *iptr,
//                                    size_t texOffset )

__global__ void copy_const_kernel(float *iptr, cudaTextureObject_t texConstSrc) {
    // map from threadIdx/BlockIdx to pixel position
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    int offset = x + y * blockDim.x * gridDim.x;

    float c = tex1Dfetch<float>(texConstSrc, offset);
    if (c != 0) iptr[offset] = c;
}

// Updated setup for texture objects
void setup_texture(int imageSize, float *dev_inSrc, float *dev_outSrc, float *dev_constSrc) {
    cudaResourceDesc resDesc;
    memset(&resDesc, 0, sizeof(resDesc));
    resDesc.resType = cudaResourceTypeLinear;
    resDesc.res.linear.desc = cudaCreateChannelDesc<float>();
    resDesc.res.linear.sizeInBytes = imageSize;

    cudaTextureDesc texDesc;
    memset(&texDesc, 0, sizeof(texDesc));
    texDesc.readMode = cudaReadModeElementType;

    // Setup texture for constant source
    resDesc.res.linear.devPtr = dev_constSrc;
    cudaCreateTextureObject(&texConstSrcObj, &resDesc, &texDesc, NULL);

    // Setup texture for input source
    resDesc.res.linear.devPtr = dev_inSrc;
    cudaCreateTextureObject(&texInObj, &resDesc, &texDesc, NULL);

    // Setup texture for output source
    resDesc.res.linear.devPtr = dev_outSrc;
    cudaCreateTextureObject(&texOutObj, &resDesc, &texDesc, NULL);
}
// globals needed by the update routine
struct DataBlock {
    unsigned char *output_bitmap;
    float *dev_inSrc;
    float *dev_outSrc;
    float *dev_constSrc;
    CPUAnimBitmap *bitmap;

    cudaEvent_t start, stop;
    float totalTime;
    float frames;
};

void anim_gpu(DataBlock *d, int ticks) {
    HANDLE_ERROR(cudaEventRecord(d->start, 0));
    dim3 blocks(DIM / 16, DIM / 16);
    dim3 threads(16, 16);
    CPUAnimBitmap *bitmap = d->bitmap;
    // since tex is global and bound, we have to use a flag to
    // select which is in/out per iteration
    volatile bool dstOut = true;
    for (int i = 0; i < 90; i++) {
        float *in, *out;
        cudaTextureObject_t texIn, texOut;
        if (dstOut) {
            in = d->dev_inSrc;
            out = d->dev_outSrc;
            texIn = texInObj;
            texOut = texOutObj;
        } else {
            out = d->dev_inSrc;
            in = d->dev_outSrc;
            texIn = texOutObj;
            texOut = texInObj;
        }
        copy_const_kernel<<<blocks, threads>>>(in, texConstSrcObj);
        blend_kernel<<<blocks, threads>>>(out, dstOut, texIn, texOut);
        dstOut = !dstOut;
    }

    HANDLE_ERROR(cudaMemcpy(bitmap->get_ptr(), d->output_bitmap, bitmap->image_size(), cudaMemcpyDeviceToHost));

    HANDLE_ERROR(cudaEventRecord(d->stop, 0));
    HANDLE_ERROR(cudaEventSynchronize(d->stop));
    float elapsedTime;
    HANDLE_ERROR(cudaEventElapsedTime(&elapsedTime, d->start, d->stop));
    d->totalTime += elapsedTime;
    ++d->frames;
    printf("Average Time per frame: %3.1f ms\n", d->totalTime / d->frames);
}
// clean up memory allocated on the GPU

void anim_exit(DataBlock *d) {
    cudaDestroyTextureObject(texInObj);
    cudaDestroyTextureObject(texOutObj);
    cudaDestroyTextureObject(texConstSrcObj);

    HANDLE_ERROR(cudaFree(d->dev_inSrc));
    HANDLE_ERROR(cudaFree(d->dev_outSrc));
    HANDLE_ERROR(cudaFree(d->dev_constSrc));

    HANDLE_ERROR(cudaEventDestroy(d->start));
    HANDLE_ERROR(cudaEventDestroy(d->stop));
}

int main(void) {
    DataBlock data;
    CPUAnimBitmap bitmap(DIM, DIM, &data);
    data.bitmap = &bitmap;
    data.totalTime = 0;
    data.frames = 0;
    HANDLE_ERROR(cudaEventCreate(&data.start));
    HANDLE_ERROR(cudaEventCreate(&data.stop));

    int imageSize = bitmap.image_size();

    HANDLE_ERROR(cudaMalloc((void**)&data.output_bitmap, imageSize));
    // assume float == 4 chars in size (ie rgba)

    HANDLE_ERROR(cudaMalloc((void**)&data.dev_inSrc, imageSize));
    HANDLE_ERROR(cudaMalloc((void**)&data.dev_outSrc, imageSize));
    HANDLE_ERROR(cudaMalloc((void**)&data.dev_constSrc, imageSize));

    // intialize the constant data
    float *temp = (float*)malloc( imageSize );
    for (int i=0; i<DIM*DIM; i++) {
        temp[i] = 0;
        int x = i % DIM;
        int y = i / DIM;
        if ((x>300) && (x<600) && (y>310) && (y<601))
            temp[i] = MAX_TEMP;
    }
    temp[DIM*100+100] = (MAX_TEMP + MIN_TEMP)/2;
    temp[DIM*700+100] = MIN_TEMP;
    temp[DIM*300+300] = MIN_TEMP;
    temp[DIM*200+700] = MIN_TEMP;
    for (int y=800; y<900; y++) {
        for (int x=400; x<500; x++) {
            temp[x+y*DIM] = MIN_TEMP;
        }
    }

    // Debugging puropse of printing a few initial values
    for (int i = 0; i < 10; i++) {
        printf("%f ", temp[i]);
    }
    printf("\n");

    HANDLE_ERROR( cudaMemcpy( data.dev_constSrc, temp,
                              imageSize,
                              cudaMemcpyHostToDevice ) );    

    // initialize the input data
    for (int y=800; y<DIM; y++) {
        for (int x=0; x<200; x++) {
            temp[x+y*DIM] = MAX_TEMP;
        }
    }
    HANDLE_ERROR( cudaMemcpy( data.dev_inSrc, temp,
                              imageSize,
                              cudaMemcpyHostToDevice ) );
    free( temp );
    setup_texture(imageSize, data.dev_inSrc, data.dev_outSrc, data.dev_constSrc);

    bitmap.anim_and_exit((void (*)(void*, int))anim_gpu, (void (*)(void*))anim_exit);
}

