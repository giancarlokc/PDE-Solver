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

#define GAUSS_SIDEL_METHOD 1
#define OVER_RELAXATION_METHOD 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        printf("Usage: pdeSolver -nx <Nx> -ny <Ny> -i <maxIter> -m <gs | sor> -o arquivo_saida \n");
		return -1;
    }

	// Parameters
	double nx, ny, n_iterations;
	int method;
	char* output_file;

	// Read parameters
	int i=1;
	while(argc > i) {
		if(argc > i+1) {
			if(!strcmp(argv[i], "-nx")) {
				nx = strtod(argv[i+1], NULL);
			} else if(!strcmp(argv[i], "-ny")) {
				ny = strtod(argv[i+1], NULL);
			} else if(!strcmp(argv[i], "-i")) {
				n_iterations = atoi(argv[i+1]);
			} else if(!strcmp(argv[i], "-m")) {
				if(!strcmp(argv[i+1], "gs")){
					method = GAUSS_SIDEL_METHOD;
				} else if(!strcmp(argv[i+1], "sor")){
					method = OVER_RELAXATION_METHOD;
				} else {
					printf("Invalid method: %s\n", argv[i+1]);
					return -1;
				}
			} else if(!strcmp(argv[i], "-o")) {
				output_file = argv[i+1];
			} else {
				printf("Unkown parameter: %s\n", argv[i]);
				return -1;
			}
		} else {
			printf("Value for parameter '%s' was not defined\n", argv[i]);
			return -1;
		}
		
		i+=2;
	}
    
    // Calculate hx and hy
    double hx = DOMAIN_LENGTH_X / nx;
    double hy = DOMAIN_LENGTH_Y / ny;
    #if DEBUG
        printf("HX = %f\n", hx);
        printf("HY = %f\n", hy);
    #endif
    
}
