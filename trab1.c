// UFPR - Iniciação a Computação Científica 2015

// Output file:
//  X1 Y1 Z1
//  X2 Y2 Z2
//  .  .  .
//  .  .  .
//  .  .  .
//  Xn Yn Zn

// F(x) = 4*(PI)^2 * sin(2*PI*x) * sinh(2*PI*y)
// Domain: x = [0,2] , y = [0,1]

#define DEBUG 1

#include <stdio.h>
#include <stdlib.h>

int error(char* error_mesg, int line) {
    printf("ERROR: %s", error_mesg);
    if(DEBUG) {
        printf(" (AT LINE %d)", line);
    }
    printf("\n");
    return -1;
}

int main(int argc, char **argv) {
    // Usage
    if(argc < 4) {
        printf("Usage: trab1 [N x] [N y] [Number of iterations]\n");
    }
    
    // Get and check parameters
    double nx = strtod(argv[1], NULL);
    if(nx == 0.0) {
        return error("[N x] can NOT be zero", __LINE__);
    }
    double ny = strtod(argv[2], NULL);
    if(ny == 0.0) {
        return error("[N y] can NOT be zero", __LINE__);
    }
    double n_iterations = strtod(argv[3], NULL);
    if(n_iterations == 0.0) {
        return error("[Number of iterations] can NOT be zero", __LINE__);
    }
    
    // Calculate hx and hy
    
    
}
