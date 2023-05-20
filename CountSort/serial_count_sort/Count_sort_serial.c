/* Assignment 3: Problem 1 (Count sort algorithm serial)
Name: Anuradha Agarwal
Class: COMP 605, Scientific computing
Location: /home/605/agarwa/assign3/CountSort/count_sort_serial/
Last Updated: March 19, 2023
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void Countsort(int a[], int n) 
{
    int i, j, count;
    int *temp = (int*) malloc(n * sizeof(int));

    for (i = 0; i < n; i++) 
    {
        count = 0;

        for (j = 0; j < n; j++) 
        {
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        }

        temp[count] = a[i];
    }

    memcpy(a, temp, n * sizeof(int));

    free(temp);
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) {
        printf("Please provide the length of numbers you want to be sorted.");
        exit(1);
    }
    
    int n = strtol(argv[1], NULL, 10);
    int a[n];

    
     // printf("Unsorted array: ");
     for (int i = 0; i < n; i++) {
        a[i] = rand() % 10000; // generate random numbers between 0 and 9999
        // printf("%d ", a[i]);
    }
    // printf("\n");

    struct timeval begin, end;
    gettimeofday(&begin, 0);  

    Countsort(a, n);

    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;

    /*
    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    */ 

    printf("Time taken to run serial count sort algorithm: %f seconds.\n", elapsed);    
    return 0;
}

