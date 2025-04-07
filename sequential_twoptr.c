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

    int result = 0;
    // For every element of the array
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

int main(int argc, char *argv[])
{
    int n;
    int *arr;

    if (argc != 2)
    {
        fprintf(stderr, "\nIncorrect number of arguments\n\t---USAGE: ./seq_naive <array size>\n\n");
        exit(1);
    }
    n = atoi(argv[1]); // Get array size
    arr = (int *)malloc(n * sizeof(int));
    // Generate random array of n integers
    srand(time(NULL));
    // Array "heights" will generate between 0 and 15
    for (int i = 0; i < n; i++)
    {
        // TODO: "Random" array is generated the name for same n every time
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
    printf("Maximum trapped rainwater: %d units\n", maxWater(arr, n));

    free(arr);
    return 0;
}