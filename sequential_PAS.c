/** FILE: sequencial_PAS.c
 *  Description: Here we are implementing the sequencial code of catching rain water.this version we calculate the highest bar on the left first and on the right. we use pointers
 * to traverse the array.
 *  Compile:
 *          gcc -o spas sequential_PAS.c
 *  Execution:
 *          ./spas <array size>
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int maxWater(int arr[], int n)
{

    // Left[i] contains height of tallest bar to the
    // left of i'th bar including itself
    int left[n];

    // Right[i] contains height of tallest bar to
    // the right of i'th bar including itself
    int right[n];

    int res = 0;

    // Fill left array
    left[0] = arr[0];
    for (int i = 1; i < n; i++)
        left[i] = left[i - 1] > arr[i] ? left[i - 1] : arr[i];

    // Fill right array
    right[n - 1] = arr[n - 1];
    for (int i = n - 2; i >= 0; i--)
        right[i] = right[i + 1] > arr[i] ? right[i + 1] : arr[i];

    // Calculate the accumulated water element by element
    for (int i = 1; i < n - 1; i++)
    {
        int minOf2 = left[i - 1] < right[i + 1] ? left[i - 1] : right[i + 1];
        if (minOf2 > arr[i])
        {
            res += minOf2 - arr[i];
        }
    }

    return res;
}

void Usage(char *prog_name)
{
    fprintf(stderr, "\nIncorrect number of arguments:\n---- USAGE: %s <array size> <nThreads> ----\n\n", prog_name);
    exit(1);
}

int main(int argc, char *argv[])
{
    int num, nThreads;
    int *arr;
    clock_t start, end;
    double elapsed;

    if (argc != 2)
    {
        Usage(argv[0]);
    }

    num = atoi(argv[1]); // Get array size
    arr = (int *)malloc(num * sizeof(int));
    // Generate random array of n integers
    // srand(time(NULL));    Removed for testing consistency
    // Array "heights" will generate between 0 and 15
    for (int i = 0; i < num; i++)
    {
        arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
    }
    start = clock();
    int n = sizeof(arr) / sizeof(arr[0]);
    printf(" \n");
    printf("totalwater :%d  \n", maxWater(arr, num));
    end = clock();
    elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Elapsed time: %lf\n", elapsed);
    return 0;
}
