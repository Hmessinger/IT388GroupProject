/** FILE: mpi_naive.c
 *  Description: Here we are adding openMP to the sequential (naive) version of the trapping rainwater problem. This will parallelize the function that
 *               calculates the amount of rainwater that can be trapped. This approach will speedup the time complexity because there will be multiple
 *               threads working on the computations.
 *  Compile:
 *          gcc -o mpi_naive mpi_naive.c
 *  Execution:
 *          ./mpi_naive <array size>
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

 #include<mpi.h>
 #include <stdio.h>

 // serial part of code so far    vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
 int maxWater(int arr[], int n)
{
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
    // Array "heights" will generate between 0 and 15
    for (int i = 0; i < n; i++)
    {
        // TODO: "Random" array is generated the name for same n every time
        arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
    }

    printf("Array:\n");
    for (int i = 0; i < n; i++)
    {
        printf("[%d] ", arr[i]);
    }
    printf("\n---------------------------------------\n");
    printf("Maximum trapped rainwater: %d units\n", maxWater(arr, n));

    free(arr);
    return 0;
}
