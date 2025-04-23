/** FILE: omp_PAS.c
 *  Description: Here we are implementing the omp version of the preffix and suffix approach to the catching rain water problem.
 *          gcc -o omppas omp_PAS.c -fopenmp
 *  Execution:
 *          ./omppas <numthreads> <array size>
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

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// fill the left array  passing pointers org array and leftarray
void fillLeftAndRight(int *orig, int *left, int num, int *right) // might have to change orig to a non pointer for security
{
    // do omp sections and have the threads at least populate left and right in parallel
    left[0] = orig[0];
    right[num - 1] = orig[num - 1];
#pragma omp parallel
    {
#pragma omp sections nowait
        {
#pragma omp section
            {
                for (int i = 1; i < num; i++)
                {
                    if (left[i - 1] > orig[i])
                    {
                        left[i] = left[i - 1];
                    }
                    else
                    {
                        left[i] = orig[i];
                    }
                }
            }
#pragma omp section
            {
                for (int j = num - 2; j >= 0; j--)
                {
                    if (right[j + 1] > orig[j])
                    {
                        right[j] = right[j + 1];
                    }
                    else
                    {
                        right[j] = orig[j];
                    }
                }
            }
        }
    }
}

int maxWater(int arr[], int num)
{

    int *left; // setting size of left
    left = (int *)malloc(num * sizeof(int));
    int *right; // setting size of right
    right = (int *)malloc(num * sizeof(int));
    int res = 0;                             // amount we can have
    fillLeftAndRight(arr, left, num, right); // Fill left adn right array
#pragma omp parallel for reduction(+ : res)  // could be wrong
    for (int i = 1; i < num - 1; i++)
    {
        int minOf2 = left[i - 1] < right[i + 1] ? left[i - 1] : right[i + 1];
        if (minOf2 > arr[i])
        {
            res += minOf2 - arr[i];
        }
    }
    return res;
}

// Usage function for displaying an input error message
void Usage(char *prog_name)
{
    fprintf(stderr, "\nIncorrect number of arguments:\n---- USAGE: %s <nThreads> <array size> ----\n\n", prog_name);
    exit(1);
}

int main(int argc, char *argv[])
{
    int num, nThreads;
    int *arr;
    double start, elapsed;

    if (argc == 2)
    {
        int buildings[] = {66, 104, 184, 340, 303, 159, 204, 132, 166, 307, 60, 212, 208, 152, 91, 93, 87, 198,
                           234, 161, 96, 87, 120, 150, 66, 60, 84, 117, 121, 55, 145, 102, 62, 150, 62, 119, 152,
                           118, 137, 106, 169, 99, 139, 142, 167, 96, 148, 155, 104, 153, 74, 207, 124, 192, 107,
                           155, 174, 114, 160, 78, 91, 125, 156, 182, 102, 134, 125, 15, 15, 86, 68, 102, 169, 122,
                           139, 278, 55, 128, 30, 99, 132, 194, 249, 257, 179, 151, 126, 130, 124, 164, 130, 166,
                           110, 121, 143, 230, 91, 122, 93, 103, 146, 62, 183, 101, 108, 157, 153, 167, 198, 84,
                           141, 155, 101, 138, 158, 135, 126, 126, 130, 192, 137, 106, 158, 97, 149, 120, 87, 92,
                           86, 82, 87, 118, 99, 95, 113, 157, 137, 140, 140, 140, 140, 151, 148, 111, 138, 118,
                           134, 111, 111, 12, 112, 93, 96, 79, 106, 181, 124, 85, 129, 114, 112, 55, 103, 128, 91,
                           182, 165, 92, 66, 135, 89, 143, 209, 116, 138, 76, 185, 174, 60, 221, 120, 123, 196,
                           87, 79, 177, 152, 142, 78, 50, 20, 140, 195, 6, 4, 86, 13, 5, 10, 12, 4, 5, 6, 6, 4, 3,
                           6, 14, 6, 4, 6, 6, 6, 12, 4, 4, 8, 9, 8, 6, 8, 8, 14, 42, 23, 63, 85, 123, 89, 63, 84,
                           66, 8, 15, 12, 16, 16, 14, 17, 80, 177, 143, 169, 88, 87, 72, 163, 197, 4, 13, 13, 1,
                           132, 260, 94, 143, 10, 45, 76, 125, 222, 174, 98, 150, 229, 98, 179, 443, 11, 81, 96,
                           137, 257, 207, 189, 4, 162, 10, 3, 8, 163, 5, 183, 112, 65, 80, 234, 60, 12, 20, 5, 8,
                           3, 13, 16, 13, 3, 8, 6, 5, 4, 14, 13, 3, 17, 17, 4, 4, 5, 4, 2, 15, 16, 15, 16, 16, 12,
                           12, 15, 35, 3, 13, 7, 12, 15, 7, 5, 5, 4, 4, 4, 30, 20, 20, 20, 50, 49, 50, 54, 255,
                           20, 12, 69, 107, 240, 83, 96, 14, 122, 65, 59, 50, 116, 153, 45, 18, 10, 0, 15, 5, 29,
                           20, 125, 40, 4, 2, 15};

        num = sizeof(buildings) / sizeof(buildings[0]);
        nThreads = atoi(argv[1]); // Get number of threads
        arr = (int *)malloc(num * sizeof(int));
        // Copying the buildings array into arr
        for (int i = 0; i < num; i++)
        {
            arr[i] = buildings[i];
        }
    }
    else if (argc == 3)
    {
        nThreads = atoi(argv[1]);               // Get number of threads
        num = atoi(argv[2]);                    // Get array size
        arr = (int *)malloc(num * sizeof(int)); // Allocate memory to the array
        // Array "heights" will generate between 0 and 15
        for (int i = 0; i < num; i++)
        {
            arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
        }
    }
    else
    {
        Usage(argv[0]);
    }

    printf("N size: %d\n", num);
    start = omp_get_wtime();
    printf("Maximum trapped rainwater: %d units\n", maxWater(arr, num));
    elapsed = omp_get_wtime() - start;
    printf("Elapsed time: %lf\n", elapsed);

    free(arr);
    return 0;
}
