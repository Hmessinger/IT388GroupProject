/*
OMP only approach to the trapping rainwater problem.
Naive approach: O(n^2) double loop

Code credit: Geeks For Geeks
    https://geeksforgeeks.org/trapping-rain-water/#

This code will be modified into the parallel final code.
This is the first modification of the the sequential code. It uses only OMP to solve the problem.

Compile: gcc -o omp_naive omp_naive.c -fopenmp
Execute: ./omp_naive <array size> <nThreads>
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int maxWater(int arr[], int n, int nThreads)
{
    int result = 0;
// Parallelizing the outer for loop for the rainwater computation, if the number of values is less than 100 it will work sequentially
#pragma omp parallel for num_threads(nThreads) reduction(+ : result) if (n > 100)
    for (int i = 1; i < n - 1; i++)
    {
        // Find max to the left
        int left = arr[i];
        for (int j = 0; j < i; j++)
        {
            if (arr[j] > left)
                left = arr[j];
        }
        // Find max to the right
        int right = arr[i];
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] > right)
                right = arr[j];
        }
        // Update max water result
        result += (left < right ? left : right) - arr[i];
    }
    return result;
}

// Usage function for displaying an input error message
void Usage(char *prog_name)
{
    fprintf(stderr, "\nIncorrect number of arguments:\n---- USAGE: %s <array size> <nThreads> ----\n\n", prog_name);
    exit(1);
}

int main(int argc, char *argv[])
{
    int n, nThreads;
    int *arr;
    double start, end;

    if (argc != 3)
    {
        Usage(argv[0]);
    }
    n = atoi(argv[1]);                    // Get array size
    nThreads = atoi(argv[2]);             // Get number of threads
    arr = (int *)malloc(n * sizeof(int)); // Allocate memory to the array
    // Generate random array of n integers
    // Array "heights" will generate between 0 and 15
    for (int i = 0; i < n; i++)
    {
        arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
    }

    // Print the array if the size of it is less than 30
    if (n < 30)
    {
        printf("Array:\n");
        for (int i = 0; i < n; i++)
        {
            printf("[%d] ", arr[i]);
        }
        printf("\n---------------------------------------------\n");
    }
    else
        printf("Array too large to print\n");

    start = omp_get_wtime();
    printf("Maximum trapped rainwater: %d units\n", maxWater(arr, n, nThreads));
    end = omp_get_wtime() - start;
    printf("Elapsed time: %lf\n", end);

    free(arr);
    return 0;
}