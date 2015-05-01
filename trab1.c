// UFPR - Iniciação a Computação Científica 2015

// Output file:
//  Hx x1	x2	. . .	xn
//  y1 z11	z12 . . .	z1n			
//  y2 z21	z22 . . .	z2n
//  y3 z31	z32 . . .	z3n
//  .  	.	 .  .
//  .	.	 .	  .
//  .	.    .		.
//  yn zn1  zn2      	znn 

// F(x) = 4*(PI)^2 * sin(2*PI*x) * sinh(2*PI*y)
// Domain: x = [0,2] , y = [0,1]

#define DEBUG 1
#define DOMAIN_LENGTH_X 2
#define DOMAIN_LENGTH_Y 1

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
		return -1;
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
    double hx = DOMAIN_LENGTH_X / nx;
    double hy = DOMAIN_LENGTH_Y / ny;
    #if DEBUG
        printf("HX = %f\n", hx);
        printf("HY = %f\n", hy);
    #endif
    
}
