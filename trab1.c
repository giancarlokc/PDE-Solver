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

#define DOMAIN_START_X 0.0
#define DOMAIN_LENGTH_X 2.0
#define DOMAIN_START_Y 0.0
#define DOMAIN_LENGTH_Y 1.0

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

	// ------------------------------------------------------- INPUT

    // Usage
    if(argc < 4) {
        printf("Usage: pdeSolver -nx <Nx> -ny <Ny> -i <maxIter> -m <gs | sor> -o arquivo_saida \n");
		return -1;
    }

	// General variables
	long i, j;

	// Parameters
	long nx, ny, n_iterations;
	int method;
	char* output_file;

	// Read parameters
	i=1;
	while(argc > i) {
		if(argc > i+1) {
			if(!strcmp(argv[i], "-nx")) {
				nx = atol(argv[i+1]);
			} else if(!strcmp(argv[i], "-ny")) {
				ny = atol(argv[i+1]);
			} else if(!strcmp(argv[i], "-i")) {
				n_iterations = atol(argv[i+1]);
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

	// ------------------------------------------------------- PROCESSING
    
    // Calculate hx and hy
    double hx = DOMAIN_LENGTH_X / nx;
    double hy = DOMAIN_LENGTH_Y / ny;
    #if DEBUG
        printf("HX = %f\n", hx);
        printf("HY = %f\n", hy);
    #endif
    
    // Alloc the memory
    long n_lines = (nx+1)*(ny+1);
    long n_columns = (nx+1)*(ny+1); 
    double *mat = (double *) malloc(n_lines*n_columns*sizeof(double));
    double *matB = (double*) malloc(n_lines*sizeof(double));
    #if DEBUG
        printf("Vector instanciated with %ld positions.\n", n_lines*n_columns);
    #endif
    
    // Set initial values
    for(i=0;i<n_lines*n_columns;i+=n_columns){
        for(j=0;j<n_columns;j++) {
            if(i/n_columns == j) {
                // IF the point is an edge, mark the related B position with a zero ELSE mark it with a 1 (change 1 to the actual value)
                if((((i/n_columns)+(ny+1)) / (nx+1) == 1) || (((i/n_columns)+(ny+1)) / (nx+1) == nx+1) || (j % (nx+1)) == 0 || (j % (nx+1)) == nx ) {
                    matB[i/n_columns] = 0;
                } else {
                    matB[i/n_columns] = 1;
                }
                mat[i + j] = 1;
            } else {
                mat[i + j] = 0;
            }
        }
    }
    
    // Print the data
    printf("A= [\n");
    for(i=0;i<n_lines*n_columns;i+=n_columns){
        printf("    ");
        for(j=0;j<n_columns;j++) {
            printf("%f ", mat[i + j]);
        }
        printf("\n");
    }
    printf("]\n");
    printf("B=[\n");
    for(i=0;i<n_lines;i++) {
        printf("    %f\n", matB[i]);
    }
    printf("]\n");

	// ------------------------------------------------------- OUTPUT
	
	// Write the 'out.data' matrix for the plot
	FILE *fp;
	fp = fopen(output_file, "w+");
	if(fp == NULL) {
		printf("Can't create/open file %s\n", output_file);
		return -1;
	}

    // write the points in x
	fprintf(fp, " %ld", nx+1);
	double current_point = DOMAIN_START_X;
	for(i=0;i<nx+1;i++) {
		fprintf(fp, " %f", current_point);
		current_point += hx;
	}
	fprintf(fp, "\n");

	// write the correspondent point in y and the z values
	current_point = DOMAIN_START_Y;
	for(i=0;i<ny+1;i++) {
		fprintf(fp, " %f", current_point);
		for(j=0;j<nx+1;j++) {
			fprintf(fp, " 0.0");
		}
		fprintf(fp, "\n");
		current_point += hy;
	}

	
	fclose(fp);


















}
