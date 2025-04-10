/** FILE: omp_twoptr.c
 *  Description: Here we are implementing the OMP version of the trapping rainwater problem. 
 *  Compile:
 *          gcc -o omp_tp omp_twoptr.c -fopenmp
 *  Execution:
 *          ./omp_tp <array size> <nThreads>
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

/*
OMP approach to the trapping rainwater problem.
Two Pointer Approach: O(n)

Code credit: Geeks For Geeks
    https://geeksforgeeks.org/trapping-rain-water/#

This code will be modified into the parallel final code.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int maxWater(int arr[], int n, int nThreads)
{ 
    int left;
    //int left = 1;
    int right = n-2;
    int result = 0;
    int lMax = arr[left-1];
    int rMax = arr[right+1];

    /*
        TODO: 
        For loop is not going to work
            >Performance is just worse
            >Race condition in the left++ ruins the logic
                --Can't get rid of it; OpenMP disallows (;left<=right;)
        While loop while likely have to be replaced with OMP tasks
            >OMP Single to prevent duplicate execution
            >Private and critical/atomic, etc. to prevent race condtitions
    */

    //COPY Sequential code in here and make modifications
}

int main(int argc, char *argv[])
{
    int n, nThreads;
    int *arr;
    clock_t start, end;
    double elapsed;

    if (argc != 3)
    {
        fprintf(stderr, "\nIncorrect number of arguments\n\t---USAGE: ./omp_tp <array size> <nThreads>\n\n");
        exit(1);
    }
    n = atoi(argv[1]); // Get array size
    nThreads = atoi(argv[2]); //Get number of threads
    arr = (int *)malloc(n * sizeof(int));
    // Generate random array of n integers
    //srand(time(NULL));        Removed for testing consistency
    // Array "heights" will generate between 0 and 15
    for (int i = 0; i < n; i++)
    {
        arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
    }
    
    if(n < 30)
    {
        printf("Array:\n");
        for(int i=0; i<n; i++)
        {
            printf("[%d] ",arr[i]);
        }
    }
    else
        printf("Array too large to print\n");
    printf("\n---------------------------------------\n");
    start = clock();
    printf("Maximum trapped rainwater: %d units\n", maxWater(arr, n, nThreads));
    end = clock();
    elapsed = ((double) (end-start)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %lf\n", elapsed);
    free(arr);
    return 0;
}