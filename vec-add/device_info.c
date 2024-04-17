#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>


// GPT3.5
int main() {
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint num_devices, num_platforms;
    cl_int ret;

    // Get number of OpenCL platforms
    ret = clGetPlatformIDs(0, NULL, &num_platforms);
    if (ret != CL_SUCCESS) {
        printf("Error getting number of platforms\n");
        return 1;
    }

    printf("Number of OpenCL platforms: %d\n", num_platforms);

    // Get platform IDs
    cl_platform_id *platform_ids = (cl_platform_id *)malloc(num_platforms * sizeof(cl_platform_id));
    ret = clGetPlatformIDs(num_platforms, platform_ids, NULL);
    if (ret != CL_SUCCESS) {
        printf("Error getting platform IDs\n");
        return 1;
    }

    // Iterate through each platform
    for (cl_uint i = 0; i < num_platforms; i++) {
        printf("\nPlatform %d:\n", i+1);

        // Get number of devices for this platform
        ret = clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
        if (ret != CL_SUCCESS) {
            printf("Error getting number of devices for platform %d\n", i+1);
            continue;
        }

        printf("Number of devices: %d\n", num_devices);

        // Get device IDs
        cl_device_id *device_ids = (cl_device_id *)malloc(num_devices * sizeof(cl_device_id));
        ret = clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, num_devices, device_ids, NULL);
        if (ret != CL_SUCCESS) {
            printf("Error getting device IDs for platform %d\n", i+1);
            continue;
        }

        // Iterate through each device
        for (cl_uint j = 0; j < num_devices; j++) {
            printf("\nDevice %d:\n", j+1);

            // Print device info
            char buffer[1024];
            ret = clGetDeviceInfo(device_ids[j], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL);
            if (ret != CL_SUCCESS) {
                printf("Error getting device name for device %d\n", j+1);
                continue;
            }
            printf("Device Name: %s\n", buffer);

            ret = clGetDeviceInfo(device_ids[j], CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL);
            if (ret != CL_SUCCESS) {
                printf("Error getting device vendor for device %d\n", j+1);
                continue;
            }
            printf("Device Vendor: %s\n", buffer);
        }

        free(device_ids);
    }

    free(platform_ids);

    return 0;
}

