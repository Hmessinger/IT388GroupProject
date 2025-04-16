/** FILE: omp_twoptr.c
 *  Description: Here we are implementing the omp version of the two pointer approach to the catching rain water problem.
 *          gcc -o omptwoptr omp_twoptr.c
 *  Execution:
 *          ./omptwoptr <array size>
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

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int maxWater(int arr[], int n)
{

    // TODO:
    //   Implement sequential Two Pointer code,
    //   parallelize at home later
    int left = 1;
    int right = n - 2;
    int result = 0;
    int lMax = arr[left - 1];
    int rMax = arr[right + 1];

    while (left <= right)
    {
#pragma omp tasks shared(rMax, lMax, left, right)
        if (rMax <= lMax)
        {
            result += (rMax - arr[right]) > 0 ? (rMax - arr[right]) : 0;
            rMax = rMax > arr[right] ? rMax : arr[right];
            right -= 1;
        }
        else
        {
            result += (lMax - arr[left]) > 0 ? (lMax - arr[left]) : 0;
            lMax = lMax > arr[left] ? lMax : arr[left];
            left += 1;
        }
    }

    return result;
}

int main(int argc, char *argv[])
{
    int n, nThreads;
    int *arr;
    clock_t start, end;
    double elapsed;

    if (argc != 3)
    {
        fprintf(stderr, "\nIncorrect number of arguments\n\t---USAGE: ./seq_tp <array size>\n\n");
        exit(1);
    }
    n = atoi(argv[1]);        // Get array size
    nThreads = atoi(argv[2]); // Get number of threads
    omp_set_num_threads(nThreads);
    arr = (int *)malloc(n * sizeof(int));
    // Generate random array of n integers
    // srand(time(NULL));        Removed for testing consistency
    // Array "heights" will generate between 0 and 15
    for (int i = 0; i < n; i++)
    {
        arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
    }

    if (n < 30)
    {
        printf("Array:\n");
        for (int i = 0; i < n; i++)
        {
            printf("[%d] ", arr[i]);
        }
    }
    else
        printf("Array too large to print\n");
    printf("\n---------------------------------------\n");
    start = clock();
#pragma omp parallel shared(n)
    {
#pragma omp single
        printf("Maximum trapped rainwater: %d units\n", maxWater(arr, n));
    }
    end = clock();
    elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %lf\n", elapsed);
    free(arr);
    return 0;
}