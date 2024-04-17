__kernel void vector_add(__global const float *a,
                         __global const float *b,
                         __global float *result,
                         const unsigned int n) {
    
		int i = get_global_id(0);
    
		if (i < n) {
			result[i] = a[i] + b[i];
		}
}

