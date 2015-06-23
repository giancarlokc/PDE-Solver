// UFPR - Iniciação a Computação Científica 2015

// Output file:
//  Hx x1   x2  . . .   xn
//  y1 z11  z12 . . .   zn1         
//  y2 z21  z22 . . .   zn2
//  y3 z31  z32 . . .   zn3
//  .   .    .  .
//  .   .    .    .
//  .   .    .      .
//  ym z1m  z2m         znm 

// F(x) = 4*(PI)^2 * sin(2*PI*x) * sinh(2*PI*y)
// Domain: x = [0,2] , y = [0,1]

#define DEBUG 0

#define DOMAIN_START_X 0.0
#define DOMAIN_LENGTH_X 2.0
#define DOMAIN_START_Y 0.0
#define DOMAIN_LENGTH_Y 1.0

#define GAUSS_SEIDEL_METHOD 1
#define OVER_RELAXATION_METHOD 2    

#define ERROR 0.05

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Time measure function
double timestamp(void){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return((double)(tp.tv_sec + tp.tv_usec/1000000.0));
}

int compare(double a, double b) {
    printf("Comparing %lf with %lf\n", a, b);
    if(a >= (b - b*ERROR) && a <= (b + b*ERROR)) {
        return 1;
    } else {
        return 0;
    }
}

long nx, ny, n_iterations;
long n_lines;

short int a(int i) {
    if (i < ny) {
        return 0;
    }

    if (i > n_lines-(ny+2)) {
        return 0;
    }

    if ((i-ny)%(ny+1) <= 1) {
        return 0;
    }

    return 1;
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
    int method = GAUSS_SEIDEL_METHOD;
    char* output_file;
    output_file = (char*) malloc(sizeof(char) * 100);
    strcpy(output_file, "solution.txt");
    n_iterations = 5;

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
                    method = GAUSS_SEIDEL_METHOD;
                } else if(!strcmp(argv[i+1], "sor")){
                    method = OVER_RELAXATION_METHOD;
                } else {
                    printf("Invalid method: %s\n", argv[i+1]);
                    return -1;
                }
            } else if(!strcmp(argv[i], "-o")) {
                if(strlen(argv[i+1]) > 98) {
                    printf("Output path too long (MAX_LENGTH=100)\n");
                    return -1;
                }
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

    // General variables
    n_lines = (nx+1)*(ny+1);
    double Fxy;
    double ssh;
    double gs_time = 0;
    double sor_time = 0;
    double residue_time = 0;

    // Calculate hx and hy
    double hx = DOMAIN_LENGTH_X / nx;
    double hy = DOMAIN_LENGTH_Y / ny;

    // Calculate DELTA, deltaX and deltaY
    double delta = (2/(hx*hx)) + (2/(hy*hy)) + (4*M_PI*M_PI);
    double deltax = (-1/(hx*hx))/delta;
    double deltay = (-1/(hy*hy))/delta;

    #if DEBUG
        printf("HX = %f\n", hx);
        printf("HY = %f\n", hy);
        printf("Delta = %lf\n", delta);
        printf("Delta X = %lf\n", deltax);
        printf("Delta Y = %lf\n", deltay);
    #endif

    // Alloc the memory for the equation (Ax = B) and for the residue
    // A uses unsigned char because we really don't need more than a byte
    unsigned char *A = (unsigned char *) malloc(n_lines*sizeof(unsigned char));
    double *B = (double*) malloc(n_lines*sizeof(double));
    double *x = (double*) malloc((n_lines+((ny+1)*2))*sizeof(double));
    memset (x, 0.0, (n_lines+((ny+1)*2))*sizeof(double));
    double *residue = (double *) malloc(n_lines*sizeof(double));
    double *normVector = (double *) malloc(n_iterations*sizeof(double));
    
    #if DEBUG
        printf("Vector instanciated with %ld positions.\n", n_lines);
    #endif

    // Set initial values for the matrix A and vector B
    for(i=0;i<n_lines;++i){
        A[i]=0;
        //printf("Line %ld:    point:%ld\n", i, i);
        ssh = sin(2*M_PI*floor(i/(ny+1))*hx) * sinh(2*M_PI*(i%(ny+1))*hy);
        Fxy = ((4*M_PI*M_PI) * ssh)/delta;

        #if DEBUG
            printf("Fxy = %lf * %lf\n", (4*M_PI*M_PI), ssh);
        #endif

        
        // IF the position is the edge where y = 1, set related B position with value `ssh`
        if ( (i%(ny+1))*hy == DOMAIN_LENGTH_Y )  {
            B[i] = ssh;
        // IF the position is the edge where y = 0, x = 0 or x = 2, set related B position with value `0`
        } else if (floor(i/(ny+1))*hx == DOMAIN_LENGTH_X) {
            B[i] = 0;
        } else if (floor(i/(ny+1))*hx == DOMAIN_START_X
                    || (i%(ny+1))*hy == DOMAIN_START_Y ) {
            B[i] = 0;
        // If the point is not an edge, set the matrix A line and the related B position
        } else {
            // A[i] has deltax and deltay
            A[i] = 1;
            B[i] = Fxy;
        }

    }

    /*for (i=0; i<n_lines; ++i) {
        printf("%d\n", A[i]);
    }*/

    // Linear system solving
    double soma;
    double temp;
    
    double temp1, temp2, temp3, temp4;
    double temp5, temp6, temp7, temp8;

    long k;
    double initial_time;

    for (k=0; k<n_iterations; ++k) {

        if(method == GAUSS_SEIDEL_METHOD) {
            initial_time = timestamp();

            // Start gs method
            for(i=0; i<n_lines%2; ++i) {
                temp1 = 0;
                temp2 = 0;
                temp3 = 0;
                temp4 = 0;

                if(A[i] == 1) {
                    temp1 = deltax*x[i - (ny+1)];
                    temp2 = deltax*x[i + (ny+1)];
                    temp3 = deltay*x[i - 1];
                    temp4 = deltay*x[i + 1];
                }

                x[i] = B[i] - temp1 - temp2 - temp3 - temp4;
            }

            for(i=i; i<n_lines; i+= 2) {
                temp1 = 0;
                temp2 = 0;
                temp3 = 0;
                temp4 = 0;
                temp5 = 0;
                temp6 = 0;
                temp7 = 0;
                temp8 = 0;

                if(A[i] == 1) {
                    temp1 = deltax*x[i - (ny+1)];
                    temp2 = deltax*x[i + (ny+1)];
                    
                    temp3 = deltay*x[i - 1];
                    temp4 = deltay*x[i + 1];
                }
                if(A[i+1] == 1) {
                    temp5 = deltax*x[i+1 - (ny+1)];
                    temp6 = deltax*x[i+1 + (ny+1)];
                    
                    temp7 = deltay*x[i+1 - 1];
                    temp8 = deltay*x[i+1 + 1];
                }  

                x[i] = B[i] - temp1 - temp2 - temp3 - temp4;
                x[i+1] = B[i+1] - temp5 - temp6 - temp7 - temp8;
            }

            gs_time += timestamp() - initial_time;
        } else {
            fprintf(stderr, "SOR method was not implemented\n");
            return 1;
        }

        initial_time = timestamp();

        // Calculate the residue norm L2
        double norm = 0;
        double sum1, sum2, sum3, sum4;
        double sum5, sum6, sum7, sum8;
        // Calculate A*x
        for(i=0;i<n_lines%2;++i) {
            sum1 = 0;
            sum2 = 0;
            sum3 = 0;
            sum4 = 0;

            if(A[i] == 1) {
                sum1 = deltax*x[i - (ny+1)];
                sum2 = deltax*x[i + (ny+1)];
                
                sum3 = deltay*x[i - 1];
                sum4 = deltay*x[i + 1];
            }
            residue[i] = B[i] - x[i] - sum1 - sum2 - sum3 - sum4;
        }

        for(i=i;i<n_lines;i+=2) {
            sum1 = 0;
            sum2 = 0;
            sum3 = 0;
            sum4 = 0;
            sum5 = 0;
            sum6 = 0;
            sum7 = 0;
            sum8 = 0;

            if(A[i] == 1) {
                sum1 = deltax*x[i - (ny+1)];
                sum2 = deltax*x[i + (ny+1)];
                
                sum3 = deltay*x[i - 1];
                sum4 = deltay*x[i + 1];
            }
            if(A[i+1] == 1) {
                sum5 = deltax*x[i+1 - (ny+1)];
                sum6 = deltax*x[i+1 + (ny+1)];
                
                sum7 = deltay*x[i+1 - 1];
                sum8 = deltay*x[i+1 + 1];
            }
            residue[i] = B[i] - x[i] - sum1 - sum2 - sum3 - sum4;
            residue[i+1] = B[i+1] - x[i+1] - sum5 - sum6 - sum7 - sum8;
        }

        // Calculate norm L2 = ||B - Ax||2
        for(i=0;i<n_lines;++i) {
            norm += residue[i]*residue[i];
        }
        norm = sqrt(norm);
        #if DEBUG
            printf("Norm for iteration %ld: %.15lf\n", k, norm);
        #endif
        normVector[k] = norm;

        residue_time += timestamp() - initial_time;
    }

    // ------------------------------------------------------- OUTPUT

    // Write the configuration file for the plot
    FILE *fp;
    fp = fopen(output_file, "w+");
    if(fp == NULL) {
        printf("Can't create/open file %s\n", output_file);
        return -1;
    }

    fprintf(fp, "###########\n");
    // Write mean of iterations for each method
    if(method == GAUSS_SEIDEL_METHOD) {
        printf("%lf\n", gs_time/n_iterations);
        fprintf(fp, "# Tempo método Gauss-Seidel: %lf\n", gs_time/n_iterations);
    } else if(method == OVER_RELAXATION_METHOD) {
        printf("%lf\n", gs_time/n_iterations);
        fprintf(fp, "#%lf\n", gs_time/n_iterations);
    }

    // Write mean for calculating the residue
    printf("# Tempo Resíduo: %lf\n", residue_time/n_iterations);
    fprintf(fp, "# Tempo Resíduo: %lf\n", residue_time/n_iterations);

    fprintf(fp, "#\n# Norma do Resíduo\n");

    // Write norms
    for(i=0;i<n_iterations;++i) {
        fprintf(fp, "# i=%ld: %lf\n", i+1, normVector[i]);
    }
    fprintf(fp, "###########\n");

    // write gnuplot configuration
    fprintf(fp, "set title \"3D surface from a grid (matrix) of Z values\"\n");
    fprintf(fp, "set xrange [ 0.0 : 2.0 ]\n");
    fprintf(fp, "set yrange [ 0.0 : 1.0 ]\n");
    fprintf(fp, "set ticslevel 0\n");
    fprintf(fp, "splot \"out.data\" matrix nonuniform with lines t ''\n");

    // Save and close file
    fclose(fp);

    // Write the 'out.data' matrix for the plot
    fp = fopen("out.data", "w+");
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

    // write the points in y
    current_point = DOMAIN_START_Y;
    for(i=0;i<ny+1;i++) {
        fprintf(fp, " %f", current_point);

        // Write the correspondent z points
        for(j=0;j<nx+1;j++) {
            fprintf(fp, " %lf", x[(j*(ny+1))+i]);
        }

        fprintf(fp, "\n");
        current_point += hy;
    }
    
    // Save and close file
    fclose(fp);
}
