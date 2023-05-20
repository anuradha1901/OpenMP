/*
Name: Anuradha Agarwal
Class: COMP 605 Scientific computing
Assignment 3: Linear System Row Oriented
Location: /home/605/agarwa/assign3/LinearSystem
Last updated: March 24, 2023
Notes: Using the assignment template for the function
*/


// Importing all the necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>

// Defining the functions used
// Here the function back_substitution is implemented in parallel using OpenMP
// The overall template is from the assignment.
void back_substitution(int n, double *A[], double *b, double *x, int thread_count);

// The main code starts here
int main(int argc, char* argv[]) {
        // checking the number of output 
        if (argc < 3) {
                printf("Please enter the matriz size as your first argument and number of threads as second \n", argv[0]);
                exit(1);
        }

        int n = strtol(argv[1], NULL, 10);                          // size of the matrix
        int thread_count = strtol(argv[2], NULL, 10);               // number of threads

	printf("Row oriented calculation \n");
	// Reference: https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
        // initializing matrix A using malloc and calloc 
        // this would also initialize the matrix to all zeros
        double **A = (double **)malloc(n * sizeof(double *));
        for (int i = 0; i < n; i++) {
                A[i] = (double *)calloc(n, sizeof(double));
        }
        double *b = (double *)calloc(n, sizeof(double));           // vector b
        double *x = (double *)calloc(n, sizeof(double));           // vector x
        int i, j;

        // Initializing the seed to 0
        srand(0);

	// I changed the scaling from 0 to 100 --> 100 to 200 after the presentation in class where
        // it was mentioned about dividing by really small values can cause large problems

	
	// Reference: https://c-for-dummies.com/blog/?p=1458
        // Generate a random upper triangular matrix
        for (i = 0; i < n; i++) {
                for (j = i; j < n; j++) {
                        A[i][j] = rand()/(double)RAND_MAX * 100 + 100;
                }
        }
        
        printf("The vector b is: \n");
        // Generating a random vector b (scaling it to 100) and printing the vector
        // I did did because I was sometimes getting very very huge numbers
        for (i = 0; i < n; i++) {
                b[i] = rand()/(double)RAND_MAX * 100 + 100;
                printf("%f ",b[i]);
        }
        printf("\n");
	printf("\n");	

        printf("The matrix A is: \n");
        // Printing the matrix
        for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                        printf("%f ", A[i][j]);
                }
                printf("\n");
        }
        
	printf("\n");

        // Set the number of threads
        omp_set_num_threads(thread_count);

        // begin time 
        struct timeval begin;
        struct timeval end;
        gettimeofday(&begin, NULL);

        // Solve the linear system using the parallel back_substitution function
        back_substitution(n, A, b, x, thread_count);

        // end time 
        gettimeofday(&end, NULL);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double totalTime = seconds + microseconds*1e-6;

        // Print the solution vector x
        printf("Solution vector x:\n");
        for (i = 0; i < n; i++) {
                printf("%f ", x[i]);
        }
        printf("\n");
	printf("\n");

        printf("Total time taken: %f seconds \n", totalTime);

	printf("\n");

        // Free dynamically allocated memory (garbage collection)
        for (int i = 0; i < n; i++) {
                free(A[i]);
        }
        free(A);
        free(b);
        free(x);

        return 0;
}

// Reference: https://www.tutorialspoint.com/cprogramming/c_passing_pointers_to_functions.html
// function back_substitution takes in the following inputs:
// - n : which is the size of the matrix 
// - A : matrix A (pointer)
// - b : vector b (pointer)
// - x : vector x (pointer) --> this is initially set to zero and would get updated within the function
// - thread_count : number of allocated threads
// The only extra information/ text added here are the pragma commands 
// The outer loop cannot be parallelized because the inner loop needs information from the outer and vice versa
// So the outer loop needs to be run only with one thread: so I used #pragma omp single 
// Whereas the inner loop can be parallelized 
void back_substitution(int n, double *A[], double *b, double *x, int thread_count) {
        int row, col;

        #pragma omp single
        for (row = n-1; row >= 0; row--) {
                x[row] = b[row];

        #pragma omp parallel for num_threads(thread_count) shared(n, A, b, x, row) schedule(runtime)
        for (col = row+1; col < n; col++) {
            // printf("Thread %d working on row %d and column %d\n", omp_get_thread_num(), row, col);
            x[row] -= A[row][col] * x[col];
        }
        x[row] /= A[row][row];
    }
}

