/** FILE: omp_PAS.c
 *  Description: Here we are implementing the omp version of the preffix and suffix approach to the catching rain water problem.
 *          gcc -o omppas omp_PAS.c
 *  Execution:
 *          ./omppas <array size>
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

/*
OMP approach to the trapping rainwater problem.
Prefix and Suffix approach: O(n)

Code credit: Geeks For Geeks
    https://geeksforgeeks.org/trapping-rain-water/#

This code will be modified into the parallel final code.

*/

#include<omp.h>
#include <stdio.h>

 //    sequecnial part of the omp pas so far.  

int maxWater(int arr[], int n) {

    // Left[i] contains height of tallest bar to the
    // left of i'th bar including itself
    int left[n];

    // Right[i] contains height of tallest bar to
    // the right of i'th bar including itself
    int right[n];

    int res = 0;

    // Fill left array
    #pragma omp parallel for                     // could be wrong
    left[0] = arr[0];
    for (int i = 1; i < n; i++)
        left[i] = left[i - 1] > arr[i] ? left[i - 1] : arr[i];

    // Fill right array

    #pragma omp parallel for                   // could be wrong
    right[n - 1] = arr[n - 1];
    for (int i = n - 2; i >= 0; i--)
        right[i] = right[i + 1] > arr[i] ? right[i + 1] : arr[i];

    // Calculate the accumulated water element by element

    #pragma omp parallel for                // could be wrong
    for (int i = 1; i < n - 1; i++) {
        int minOf2 = left[i - 1] < right[i + 1] ? left[i - 1] : right[i + 1];
        if (minOf2 > arr[i]) {
            res += minOf2 - arr[i];
        }
    }

    return res;
}

int main() {
    int num;
    int *arr;
    double start, elapsed;

    if (argc != 2)
    {
        Usage(argv[0]);
    }

    num = atoi(argv[1]); // Get array size
    arr = (int *)malloc(num * sizeof(int));
    // Generate random array of n integers
    //srand(time(NULL));    Removed for testing consistency
    // Array "heights" will generate between 0 and 15
    for (int i = 0; i < n; i++)
    {
        arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
    }
    start = omp_get_wtime();
    int n = sizeof(arr) / sizeof(arr[0]);
    elapsed=omp_get_wtime()-start;
    printf("elapsed time :   %lf ", elapsed);
    printf("%d", maxWater(arr, n));
    return 0;
}
