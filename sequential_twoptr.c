/** FILE: sequential_naive.c
 *  Description: Here we are implementing the sequential version of the trapping rainwater problem. 
 *  Compile:
 *          gcc -o seq_tp sequential_twoptr.c
 *  Execution:
 *          ./seq_tp <array size>
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

/*
Sequential approach to the trapping rainwater problem.
Two Pointer Approach: O(n)

Code credit: Geeks For Geeks
    https://geeksforgeeks.org/trapping-rain-water/#

This code will be modified into the parallel final code.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int maxWater(int arr[], int n)
{

    //TODO:
    //  Implement sequential Two Pointer code,
    //  parallelize at home later   
    int left = 1;
    int right = n-2;
    int result = 0;
    int lMax = arr[left-1];
    int rMax = arr[right+1];

    if(left <= right)
    {
        result += (rMax - arr[right]) > 0 ? (rMax - arr[right]) : 0;
        rMax = rMax > arr[right] ? rMax: arr[right];
        right -= 1;
    }
    else
    {
        result += (lMax - arr[left]) > 0 ? (lMax - arr[left]) : 0;
        lMax = lMax > arr[left] ? lMax : arr[left];
        left += 1;
    }

    return result;
}

int main(int argc, char *argv[])
{
    int n;
    int *arr;
    clock_t start, end;
    double elapsed;

    if (argc != 2)
    {
        fprintf(stderr, "\nIncorrect number of arguments\n\t---USAGE: ./seq_tp <array size>\n\n");
        exit(1);
    }
    n = atoi(argv[1]); // Get array size
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
        printf("Array too lagre to print\n");
    printf("\n---------------------------------------\n");
    start = clock();
    printf("Maximum trapped rainwater: %d units\n", maxWater(arr, n));
    end = clock();
    elapsed = ((double) (end-start)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %lf\n", elapsed);
    free(arr);
    return 0;
}