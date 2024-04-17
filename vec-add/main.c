#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define MAX_SOURCE_SIZE (0x100000)

int main() {
    // Load kernel source code
		FILE *kernel_file;
    char *kernel_source;
    size_t kernel_source_size;

    kernel_file = fopen("vector_add.cl", "r");
    if (!kernel_file) {
        fprintf(stderr, "Failed to open kernel file.\n");
        exit(1);
    }

    kernel_source = (char *)malloc(MAX_SOURCE_SIZE);
    kernel_source_size = fread(kernel_source, 1, MAX_SOURCE_SIZE, kernel_file);
    fclose(kernel_file);

    // Initialize OpenCL
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint num_devices, num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &num_devices);

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    // Create memory buffers for input and output vectors
    const int n = 10;
    float a[n], b[n], result[n];
    for (int i = 0; i < n; i++) {
        a[i] = i;
        b[i] = i * 2;
    }

    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, n * sizeof(float), NULL, &ret);
    cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, n * sizeof(float), NULL, &ret);
    cl_mem result_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, n * sizeof(float), NULL, &ret);

    // Copy input vectors to memory buffers
    ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, n * sizeof(float), a, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, n * sizeof(float), b, 0, NULL, NULL);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&kernel_source, (const size_t *)&kernel_source_size, &ret);

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&result_mem_obj);
    ret = clSetKernelArg(kernel, 3, sizeof(int), (void *)&n);

    // Execute the OpenCL kernel on the array
    size_t global_item_size = n;
    size_t local_item_size = 1;
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

    // Read the memory buffer result_mem_obj to the local variable result
    ret = clEnqueueReadBuffer(command_queue, result_mem_obj, CL_TRUE, 0, n * sizeof(float), result, 0, NULL, NULL);

    printf("Array A:\n");
    for (int i = 0; i < n; i++) {
        printf("%.2f ", a[i]);
    }
    printf("\n\n");

    printf("Array B:\n");
    for (int i = 0; i < n; i++) {
        printf("%.2f ", b[i]);
    }
    printf("\n\n");

    printf("Result:\n");
    for (int i = 0; i < n; i++) {
        printf("%.2f ", result[i]);
    }
    printf("\n");

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(a_mem_obj);
    ret = clReleaseMemObject(b_mem_obj);
    ret = clReleaseMemObject(result_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    free(kernel_source);

    return 0;
}
