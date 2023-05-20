# Assignment 3: Problem 2
## Name: Anuradha Agarwal
## Class: COMP 605 - Scientific Computing
## Location: /home/605/agarwa/assign3/LinearSystem/
## Last Updated: March 24, 2023

--

This folder contains the following files and folders:
- batch.gaussian: This is the batch file to run the linearSystemGaussian executable
- batch.linearsystem: This is the batch file to run the linearSystemRow and linearSystemColumn executables
- batch.linearsystemRuntime: This is the batch file to compare the schedule(runtime)
- linearSystemColumn.c: This file is the code for parallelized column oriented back substitution
- linearSystemColumn: This is the executable for linearSystemColumn.c
- linearSystemRow.c: This file is the code for parallelized row oriented back substitution
- linearSystemRow: This is the executable for linearSystemRow.c
- linearSystemGaussian.c: This file is the code for parallelized gaussian elimination
- linearSystemGaussian: This file is the executable for linearSystemGaussian
- README.md

--

## Description of the problem:
The goal of this problem is to solve a linear system using column oriented and row oriented back
substitution. The program initializes an upper triagular matrix where the numbers are between 100 
and 200. I made it so that values don't get divided by very small numbers and cause over flow issues. 
The program also initializes the b vector in a similar way. Ax = b is then solved using back 
substitution. This program is parallelized using OpenMP. We compare the times it takes for different 
runtime schedules and the two approaches in general.

Gaussian elimination is also done as a part of this assigment where the 5 point stencil coefficient
matrix is used so we can assume that the matrix is non-singular. The only extra part of this problem
is that upper triangular matrix is created from a regular matrix. 

--

## To run linearSystemColumn.c and linearSystemRow.c:

```
        qsub batch.linearsystem
```

## To run using different runtime schedules:

```
	qsub batch.linearsystemRuntime
```

## To run linearSystemGaussian.c

```
	qsub batch.gaussian 
```

Note: I have inserted print statements to show the matrix A, vector b and the solution. When running 
the schedule runtime batch script comment out the print statements as the size of the matrix is 10000.
All the executables need two arguments, where the first is the size of the matrix and the second is 
the number of threads.

--
## Analysis:
Overall we see that row-oriented back substitution is faster than column oriented. Both have similar
ways of parallelizing the loops! Running them on parallel is definitely faster in comparision to 
serial for bigger matrices. 

--

## Questions:
Q1: (Outer loop, row oriented) No, we cannot parallelize the outer loop in the row oriented approach.
This is due to the data dependecy. The inner loop depends on the values calculated in the outer loop. 
Also, each iteration of the outer loop depends on the previous iteration. So it is no possible to 
parallelize it.

Q2: (Inner loop, row oriented) Yes,we can parallelize the inner loop in the row oriented approach. 
As each row is independent here, the loop can be executed in parallel to fill in the values for all 
the variables it can perform all the calculations independently.

Q3: (Outer loop, column oriented) No, we cannot parallelize the outer loop in the column oriented 
approach. This is because each iteration of the outerloop depends on the proevious iteration. The 
value of x[row] is used in the inner loop. If we parallelize the outer loop we would have data 
dependency issues because of x[row].

Q4: (Inner loop, column oriented) Yes, we can parallelize the inner loop in the column oriented approach. 
This is because each iteration of the inner loop updates different elements of the array and there 
are no data dependencies.

Note: I added the pragma commands and made sure the answers stay consistent with the serial version.

Q5: Write the OpenMP program: All the files are available in the current folder
- linearSystemColumn.c: This file is the code for parallelized column oriented back substitution
- linearSystemColumn: This is the executable for linearSystemColumn.c
- linearSystemRow.c: This file is the code for parallelized row oriented back substitution
- linearSystemRow: This is the executable for linearSystemRow.c
The batch script can be used to execute them.

Q6: I used schedule(runtime) as a part of my pragma commands in my code (files mentioned in the previous
answer). The batch script that I used to change the schedule type is available in the current folder.
- batch.linearsystemRuntime: This is the batch file to compare the schedule(runtime)

Q7: If your upper triangular matrix has 10000 variables, which gives the best performance? 

When I use the row oriented approach with 100 threads and 10 as the chunk size the follwoing are the elapsed
times in seconds (We can see that wrt time Static > Guided > Dynamic). Dynamic gives the best performance.
- Static: 4.431264
- Dynamic: 4.246621
- Guided: 4.268921 

To explain here, though we see that static takes longer that guided and guided takes longer than dynamic. 
I think static did worse comparitive due to the random nature of the matrices and vector. Some threads might 
have gotten larger number compared to others. So, the work wasn't divided perfectly between threads. Guided 
performed better than static as it initially assigns larger chunks and then assigns smaller, so the resources 
are being used more efficiently and no thread is just waiting around and is taking up smaller chunks as it 
finishes up its task. Knowing that dynamic schedule did the best, it makes it more clearer that there is some 
sort of imbalance in the load in static and guided schedules. Though there is more communication in dynamic it
was able to allocate the tasks more efficiently.

When I use the column oriented approach with 100 threads and 10 as the chunk size the follwoing are the elapsed
times in seconds (We can see that wrt time Static > Dynamic > Guided). Guided gives the best performance.
- Static: 5.381702
- Dynamic: 5.180603
- Guided: 5.061054

Firstly we see that column oriented approach takes longer than row. This could be due to the hit miss situation that
was discssed in assignment 1. Where the main memory has to be access more frequently. I would argue the same reason 
as mentioned before where due to the randomness of the matrix and the vector the tasks are not completely equally 
divided amongst the threads while using the static schedule. Seeing guided and dynamic do better we can conclude 
that the load is not properly balanced in static. Guided did the best as iterations increased more number of threads
have to essentially access the main memory, and the chunk sizes go smaller so it was faster when all othe threads 
worked together.
 
Q8: The files are available in the current folder with the following names:
- linearSystemGaussian.c: This file is the code for parallelized gaussian elimination
- linearSystemGaussian: This file is the executable for linearSystemGaussian
- batch.gaussian: The batch script to run linearSystemGaussian (preset to 9*9 matrix)



