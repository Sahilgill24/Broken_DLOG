// cuda implementation of the BSGS algorithm 
// But not for BigNUM


#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

// Simple modular exponentiation on CPU
// 2 Kernels will be required for this implementation
// One for Look up table generation
// other for simultaneous search 
// Basic algorithm first, then for bigNUM using CGBN lib 
