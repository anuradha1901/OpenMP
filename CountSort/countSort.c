/* Assignment 3: Problem 1 (Count sort algorithm)
Name: Anuradha Agarwal
Class: COMP 605, Scientific computing
Location: /home/605/agarwa/assign3/CountSort/
Last Updated: March 24, 2023
Notes: Following the hello program from class
*/


// Importing all the necessary libraries (the new one here is omp.h
// which is the library for OpenMP)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>  
#include <sys/time.h>

// Reference: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
// Comparison function for qsort()

int compare(const void* a, const void* b) {
	return (int)(*(int*)a - *(int*)b);
}


// Function given in the assignment
// this function takes in the array and the length of the array as inputs
// and gives the sorted array as the output

void Countsort(int a[], int n, int num_threads)
{
        // declaring all the variables
        int i, j, count;
        int *temp = (int*) malloc(n * sizeof(int));              // allocating memory
       

	// Reference: https://www.ibm.com/docs/en/zos/2.2.0?topic=programs-shared-private-variables-in-parallel-environment
        // and notes 
	#pragma omp parallel for num_threads(num_threads) private(i,j,count) shared(a,temp,n)

        for (i = 0; i < n; i++){
		int my_rank = omp_get_thread_num();              // thread number 
		// printf("Thread %d is working on index %d\n", my_rank, i);
                count = 0;

                for (j = 0; j < n; j++){
                        if (a[j] < a[i])
                                count++;
                        else if (a[j] == a[i] && j < i)
                                count++;
                 }

                temp[count] = a[i];
		// printf("Thread %d is done executing and count = %d\n", my_rank, count);
        }

        memcpy(a, temp, n * sizeof(int));                        // copying 
        free(temp);
}

// The main function begins here
int main(int argc, char *argv[]) {
        // checking if correct number of arguments are entered
        if (argc != 3) {
                printf("Please provide the length of the array as your first argument and number of threads as your second argument\n");
                exit(1);
        }

        // argument 1 is the length of the array and argument 2 is the number of threads
        int n = strtol(argv[1], NULL, 10);
        int thread_count = strtol(argv[2], NULL, 10);

        int a[n];                      // array for count sort
	int b[n];                      // array for qsort()
        srand(0);                      // seed random number generator

	
	// Reference: https://www.codespeedy.com/generate-a-random-array-in-c-or-cpp/
        // creating a random array between 0 and 9999
	// I initally just used the rand() function without the modulo and it gave me really huge numbers
	// So I opted to have an upper bound
        for (int i = 0; i < n; i++) {
                a[i] = rand() %  10000;
        }
	
	memcpy(b, a, n * sizeof(int));      // creating a copy of original array so it can be passed through qsort()

	// timing 
        struct timeval begin, end;
	gettimeofday(&begin, 0);

        Countsort(a, n, thread_count);      // calling the function 
        
	gettimeofday(&end, 0);
	long seconds = end.tv_sec - begin.tv_sec;
	long microseconds = end.tv_usec - begin.tv_usec;
	double elapsed = seconds + microseconds*1e-6;

	
        printf("Sorted array in ascending order is: ");
        for (int i = 0; i < n; i++) {
                printf("%d, ", a[i]);
        }
        printf("\n");
	
        
	printf("Time taken to sort the array using count_sort: %f seconds\n", elapsed);

	// calculating the time for qsort()

	struct timeval begin1, end1;
	gettimeofday(&begin1, 0);

    	qsort(b, n, sizeof(int), compare);
    	
	gettimeofday(&end1, 0);
	long seconds1 = end1.tv_sec - begin1.tv_sec;
	long microseconds1 = end1.tv_usec - begin1.tv_usec;
	double elapsed1 = seconds1 + microseconds1*1e-6;

    	printf("Time taken to sort the array using qsort(): %f seconds\n", elapsed1);
	
        return 0;
}
