# Assignment 3: Problem 1
## Name: Anuradha Agarwal
## Class: COMP 605 - Scientific Computing
## Location: /home/605/agarwa/assign3/CountSort/
## Last Updated: March 24, 2023

--

This folder contains the following files and folders:
- countSort.c : Main file for problem 1 of assignment 3
- countSort : Executable
- batch.countsort: Batch file for PBS torque system
- serial_count_sort/: This folder contains the files Count_sort_serial.c which is the serial code that
implements the count sort algorithm. The folder also contains Count_sort_serial which is the executable
and the batch file to execute it. 
- README.md

--

## Description of the problem:
The main code here which is countSort.c implements the count sort algorithm in C using OpenMP. Along 
with that I will be comparing it's performance with that of qsort function which is an inbuilt c function. 
The program takes two command line arguments where the first one is the length of the array to be sorted 
and the second is the number of threads that you want to use for the parallelization. 

The program generates a random array of integers, I made sure that the numbers don't go too big by using 
the modulo operator. As of now the numbers are between 1 and 9999 and can be changed to any other number. 
The goal of this problem is to understand the working of OpenMP and compare the performance of parallelized 
code with qsort and the serial version. 

--

## To run:

```
        qsub batch.countsort
```

Note: The length of the array is preset to 10000 and the number of threads is present to 10. This can 
be changed by updating the last line (line 30) after ./countsort, for each run of the qsub a new output 
file is produced with the sorted array and the time taken for qsort and count sort OpenMP.
 
--
## Analysis:

Below is an output when the length of the array is 5 and number of threads are 3. We can see how each
thread has been allocated indices and each thread goes though the inner loop finds the count and finally
ends with updating the temp array. It is interesting to see that each thread is sort of working like a 
group in terms of the order  but this needs more looking into to make sure the threads are running 
simultaneously. But we can see that each thread is working on a different index and updating a different 
location of the temp array. 

Thread 1 is working on index 2
Thread 1 is done executing and count = 1
Thread 1 is working on index 3
Thread 1 is done executing and count = 2
Thread 0 is working on index 0
Thread 0 is done executing and count = 4
Thread 0 is working on index 1
Thread 0 is done executing and count = 0
Thread 2 is working on index 4
Thread 2 is done executing and count = 3

--

## Questions: 

Q1: If one tries to parallelize the outer loop, which variables should be private and which shared?

Ans: If one tries to parallelize the outer loop the array temp, n and array a should be shared, this is because 
different threads need to access the array to populate it depending on their turn. If temp was private
then each thread will have a new array which it will be updating and sorting a different part of the 
initial input array. This would cause errors as all threads are not working to sort the complete array. The array
a is also shared as all threads need to have access to the array to loop over it and similarly all threads need
to know the length of the array to loop over.

The variables i, j and count should be private. This is so that there is no iterference with other threads,
If these variables are all shared then all threads would have access to i, j and count, and will modify the 
same variables at different instances leading to incorrect solutions. For example if 2 threads are updating
the vale for count at the same time, one would overwrite the other leading to incorrect sorting. 

To avoid incorrect sorting we need to make sure that only temp is a shared variable and i, j and count are 
all private.
--

Q2: No, there are no loop dependencies in the outer loop of the function (parallelized), it only goes through the array a
which has the unsorted list of elements and updates the temp array according to the inner iteration. As 
mentioned in the notes loop carried dependency occurs when a current variable depends on the previous iteration. 
Since we are also declaring i,j and count to be private and the array a and temp to be shared we don't have to worry 
about any incorrect results. If we didn't specify count to be private we might have to use the reduction clause. Here
each thread has its own copy of i, j and count.

In the inner loop we have data dependency when not parallelized. We do not want to parallelize the inner loop due to the
updation of the count and inner loop being dependent on the value of the variable i. Essesntially each thread
needs to go through the entire loop to find the value of count. We do not want race conditions due to count. As mentioned
earlier if count was not specified as private we have to use the reduction clause to make sure each thread has
its own count variable.
--

Q3: From [C library function: memcpy](https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm) 
I understand that this function copies n characters from memory area temp to memory area a. From my understanding 
of how OpenMP works, I don't think it can be parallelized using OpenMP as there is no loop that is involved here. 
From doing some search on the internet I found that other parallel programming frameworls like CUDA can parallelize 
this. I also would assume that if I were to parallelize something that invoves memory copy it would introduce race 
conditions similar to the one we were taught with semaphores and might lead to incorrect results. 
--

Q4: countSort.c is available at /home/605/agarwa/assign3/CountSort/
--

Q5: In the table below all times are in seconds. We see that in all the cases of lenght serial code has done way better
than OpenMP this could again be due to the size of the problem or too much of communication overhead. We see that serial 
ans parallel in our cases bith take about the same amount of time even though serial is definitely faster. This shows 
that increasing the number of threads for this problem
for those input sizes is not really benifitial.


| Threads -> |  serial   |     2     |     4     |    16     |    64     |    128    |
|            |           |           |           |           |           |           |
|        |   |           |           |           |           |           |           |
| Length v   |           |           |           |           |           |           |
|------------|-----------|-----------|-----------|-----------|-----------|-----------|
|    10000   | 0.501355  | 0.562385  | 0.562636  | 0.562453  | 0.563905  | 0.566417  |
|    20000   | 2.007772  | 2.247506  | 2.248913  | 2.248947  | 2.250158  | 2.248712  |
|    50000   | 12.535804 | 14.030823 | 14.038797 | 14.035643 | 14.052963 | 14.038491 |
|    100000  | 50.123059 | 56.231324 | 56.231324 | 56.231286 | 56.222430 | 56.237755 |
|    150000  | 112.689737| 126.467329| 126.501260| 126.483697| 126.480531| 126.481718|
|------------|-----------|-----------|-----------|-----------|-----------|-----------|


--

Q6: In the table below all time are in seconds. Each element is the time it took for the program to complete running
for a combination of thread and length of the array. On the whole we see that qsort() function is always faster
than using OpenMP atleast for the lengths 10000,20000, 50000, 100000 and 200000. We see that qsort() is way faster
than the parallelized count sort. We alseo see that as number of threads increase we don't see a major improvement 
in time. This could again be a similar issue as last assignment where the problem size is too small for us to actually
see any improvement in using more threads or the communication is increasing faster than the increase in threads. 
We see that as the length of the array increases the time increase more that the incease in length. Sometimes we
do see a decrease in using more number of threads but it is very slight, for example with 64 threads it takes
2.250158 seconds to sort an array of 20000 values but it takes 2.248712 seconds to using 64 threads. We see a
similar trend when the lenght of the array is 150000 and the threads increase from 4 to 16. Keeping the length
constant we see that most of the time, the time taken is increasing. For higher values of lenghts
the time decreases sometimes and increases sometimes but very slightly.


| Threads -> | qsort()  |     2     |     4     |    16     |    64     |    128    |
|            |          |           |           |           |           |           |
|        |   |          |           |           |           |           |           |
| Length v   |          |           |           |           |           |           |
|------------|----------|-----------|-----------|-----------|-----------|-----------|
|    10000   | 0.001134 | 0.562385  | 0.562636  | 0.562453  | 0.563905  | 0.566417  |
|    20000   | 0.002391 | 2.247506  | 2.248913  | 2.248947  | 2.250158  | 2.248712  |
|    50000   | 0.006392 | 14.030823 | 14.038797 | 14.035643 | 14.052963 | 14.038491 |
|    100000  | 0.013274 | 56.231324 | 56.231324 | 56.231286 | 56.222430 | 56.237755 |
|    150000  | 0.020282 | 126.467329| 126.501260| 126.483697| 126.480531| 126.481718|
|------------|----------|-----------|-----------|-----------|-----------|-----------|

In conclusion, qsort() and the serial count sort do faster than the parallelized version.
We also see that qsort() does better overall! 
