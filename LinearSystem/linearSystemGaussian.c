/*
Name: Anuradha Agarwal
Class: COMP 605 Scientific computing
Assignment 3: Gaussian Elimination
Location: /home/605/agarwa/assign3/LinearSystem
Last updated: March 24, 2023
Notes: Using the assignment template for the function
*/

// Importing necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>

// The create_coefficient_matrix takes in the following inputs
// - n: for the size matrix
// - A: matrix of size n*n
// It outputs the coefficient matrix of implicit method to solve 2d pde
// Reference: https://folk.ntnu.no/leifh/teaching/tkt4140/._main055.html
// Reference: https://www.mathworks.com/matlabcentral/fileexchange/38088-diffusion-in-1d-and-2d
// I also have used this earlier to solve my 2D diffusion PDE on matlab
void nonSingularMatrix(int n, double A[][n*n], int thread_count) {
        int i, j, k;
        // Initialize the matrix with zeros
        for (i = 0; i < n*n; i++) {
		for (j = 0; j < n*n; j++) {
                	A[i][j] = 0.0;
            	}
        }
        // looping through each row and column

        #pragma omp parallel for num_threads(thread_count)
        for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                        k = i*n + j;                 // linear index
                        A[k][k] = -4;                // diagonal elements to be -4
                        if (i > 0) {
                            A[k][k-n] = 1;           // setting the above entry to 1
                        }
                        if (i < n-1) {
                            A[k][k+n] = 1;           // setting the below entry to 1
                        }
                        if (j > 0) {
                            A[k][k-1] = 1;           // left
                        }
                        if (j < n-1) {
                            A[k][k+1] = 1;           // right
                        }
                }
        }
}

/*
The function gaussian elimination takes in size n, matrix A and vector b
it returns an identity matrix along with updated vector b which is our
final answer.
The following references were used:
-https://www.codewithc.com/c-program-for-gauss-elimination-method/#:~:text=The%20C%20program%20for%20Gauss,to%20the%20Gauss%20Jordan%20method.
-https://www.geeksforgeeks.org/gaussian-elimination/
*/
void gaussian_elimination(int n, double A[][n], double b[], int thread_count) {
    int i, j, k;
    double factor;

    // I repeat every process done to the matrix A to the vector b
    // I tried making everything one matrix and then go over the final matrix to get the solution
    // But I kept getting too many errors


    for (i = 0; i < n; i++) {
            // normalize the pivot row
            // This step is to make sure all the diagonal elements are 1
            factor = A[i][i];

            #pragma omp parallel for num_threads(thread_count)
            for (j = i; j < n; j++) {
                    A[i][j] /= factor;
            }
            b[i] /= factor;

            // eliminate the entries below the pivot row
            // creating the upper triangular matrix here
            // subtracting a multiple of the pivot row from each row below
            // this is so that the entries in the column below the diagonal become zero

            #pragma omp parallel for num_threads(thread_count) shared(n,A,b,i) private(k,j,factor)
            for (k = i+1; k < n; k++) {
                    factor = A[k][i];
                    for (j = i; j < n; j++) {
                        A[k][j] -= factor * A[i][j];
                    }
                    b[k] -= factor * b[i];
            }
    }
    // Updating the b vector
    // This step is essentially making the matrix a diagonal matrix with just one's
    // This here essentially is the back substitution without the dividing since
    // the diagonal elements are already 1
    // We start from the bottom row, solving for each variable
    // I end up setting the upper triangle part of zero for completeness

        #pragma omp single
        for (i = n-2; i >= 0; i--) {
                #pragma omp parallel for num_threads(thread_count) shared(n, A, b, i) private(j)
                for (j = i+1; j < n; j++) {
                        b[i] -= A[i][j] * b[j];
                        A[i][j] = 0;
                }
        }
}


int main(int argc, char *argv[]) {

        if (argc != 3) {
            printf("Please provide the size of the matrix as the first argument and number of threads as the second");
            exit(1);
        }

        int m = strtol(argv[1], NULL, 10);                          // size of the matrix
        int thread_count = strtol(argv[2], NULL, 10);               // number of threads

        srand(0);
        int n = m*m;
        double A[n][n];  // square matrix
        double b[n];     // vector b
        double x[n];     // vector x

        // creating the coefficient matrix
        nonSingularMatrix(m, A, thread_count);

        printf("Non singular matrix: \n");
        for (int i = 0; i < m*m; i++) {
                    for (int j = 0; j < m*m; j++) {
                        printf("%f ", A[i][j]);

                }
            printf("\n");
        }
        printf("\n");
        

        // creating a random b vector
        printf("Random vetor b: \n");
        for (int i = 0; i < n; i++) {
                b[i] = (double)rand()/RAND_MAX * 100 + 100;
                printf("%f ", b[i]);
            }
        printf("\n");
        printf("\n");


        // begin time
        struct timeval begin;
        struct timeval end;
        gettimeofday(&begin, NULL);

        // Perform Gaussian elimination
        gaussian_elimination(n, A, b, thread_count);
        
        // end time
        gettimeofday(&end, NULL);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double totalTime = seconds + microseconds*1e-6;

        printf("Gaussian Elimination: \n");
        for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                        printf("%f ", A[i][j]);
                }
            printf("\n");
        }

        printf("\n");

        // Solution: this is essentially the b matrix in this cases
        // As I did the back substitution within the  gaussian elimination step
        printf("Final Solution: \n");
        for (int i = 0; i < n; i++){
                printf("%f ", b[i]);
        }

        printf("\n");

        printf("Total time taken: %f seconds \n", totalTime);
        printf("\n");

        return 0;
}


