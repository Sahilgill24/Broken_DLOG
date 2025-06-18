#include <vector>
#include <stdio.h>
#include <cstdlib>

// kernel to compute our value. 
// this is called from the CPU, but runs on the GPU.
__global__ void bs_kernel(int M, int p, const int *lookup_table, int *j, int g) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < M) {
        lookup_table[j] = idx % p;
        j = (j * g) % p;
        // would this work parallely ? , not sure
        // use the pollard rho as a reference 
        // but write a basic schematic
    }
}

__global__ void gs_kernel(int *a,int g_inv, const int *lookup_table, int M, int p) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < M) {
        auto it = std::find(lookup_table.begin(), lookup_table.end(), a);
        if (it != lookup_table.end()) {
            int j = std::distance(lookup_table.begin(), it);
            // now we have the index of a in the lookup table
            // we can compute the value of j
            a = (a * g_inv) % p;
            printf("Found a at index %d, j = %d\n", idx, j);
            return j*m + lookup_table[it - lookup_table.begin()];
            
        } else {
            printf("a not found in lookup table\n");
        }

    }
}





int main() {
    const int M = 1000;
    const int p = 1000000007;
    const int g = 3;
    // this is not g_inv, this is g^m 's inverse mod p
    const int gm_inv = 333333336; 
    const int a = 5;
    int j = 1;
    std::vector<int> lookup_table(M);
    int *d_lookup_table;
    cudaMalloc(&d_lookup_table, M * sizeof(int));
    bs_kernel<<<(M + 255) / 256, 256>>>(M, p, d_lookup_table, &j, g);
    cudaMemcpy(lookup_table.data(), d_lookup_table, M * sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(d_lookup_table);
    return 0;
}