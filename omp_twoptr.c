/** FILE: omp_twoptr.c
 *  Description: Here we are implementing the OMP version of the trapping rainwater problem.
 *  Compile:
 *          gcc -o omp_tp omp_twoptr.c -fopenmp
 *  Execution:
 *          ./omp_tp <nThreads> <array size>
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
#include <math.h> //fmax function. Removes a lot of if-else statements

int maxWater(int arr[], int n, int nThreads)
{
    if (n <= 2)
        return 0; // Can't trap any water with just 2 or fewer walls
    // The two pointer approach only works in sequence.
    // This approach divides the sequential work among multiple threads.
    int result = 0;
    int right, left, rMax, lMax, rank;
    int *local_lMax = (int *)malloc(sizeof(int) * nThreads);
    int *local_rMax = (int *)malloc(sizeof(int) * nThreads);
    int *lBounds = (int *)malloc(sizeof(int) * nThreads);
    int *rBounds = (int *)malloc(sizeof(int) * nThreads);
    int work = (n + nThreads - 1) / nThreads;

// Find the local maxima for each of the parts
#pragma omp parallel num_threads(nThreads) private(left, right, rank)
    {
        rank = omp_get_thread_num();
        left = rank * work;
        right = (left + work > n ? n : (left + work));
        local_lMax[rank] = arr[left];
        for (int i = left + 1; i < right; i++)
            local_lMax[rank] = fmax(local_lMax[rank], arr[i]);
        local_rMax[rank] = arr[right - 1];
        for (int i = right - 2; i >= left; i--)
            local_rMax[rank] = fmax(local_rMax[rank], arr[i]);
    }
    // Find dependencies on borders (what if the first block in the next section would trap rainwater in this one?
    // This prevents edges of chunks from losing water)
    // These dependencies are the reason two pointer can't be fully parallelized in the first place --
    // And the restructured version to remove the dependencies is the PAS method.
    lBounds[0] = local_lMax[0];
    for (int i = 1; i < nThreads; i++)
        lBounds[i] = fmax(lBounds[i - 1], local_lMax[i]);
    rBounds[nThreads - 1] = local_rMax[nThreads - 1];
    for (int i = nThreads - 2; i >= 0; i--)
        rBounds[i] = fmax(rBounds[i + 1], local_rMax[i]);
// Now two pointer approach can be used on each of the small sections without dependency issues
// Each will be operated on by a thread - sequential work divided among parallel threads
#pragma omp parallel num_threads(nThreads) private(left, right, rank, rMax, lMax) reduction(+ : result)
    {
        rank = omp_get_thread_num();
        int start = rank * work;
        int end = (start + work > n ? n : (start + work));
        left = start;
        right = end - 1;
        lMax = arr[left];
        rMax = arr[right];

        lMax = fmax(lMax, rank > 0 ? lBounds[rank - 1] : lMax);
        rMax = fmax(rMax, rank < nThreads - 1 ? rBounds[rank + 1] : rMax);
        while (left <= right)
        {
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
    }
    free(local_lMax);
    free(local_rMax);
    free(lBounds);
    free(rBounds);
    return result;
}

// Usage function for displaying an input error message
void Usage(char *prog_name)
{
    fprintf(stderr, "\nIncorrect number of arguments:\n---- USAGE: %s <nThreads> <array size> ----\n\n", prog_name);
    exit(1);
}

int main(int argc, char *argv[])
{
    int n, nThreads;
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

        n = sizeof(buildings) / sizeof(buildings[0]);
        nThreads = atoi(argv[1]); // Get number of threads
        arr = (int *)malloc(n * sizeof(int));
        // Copying the buildings array into arr
        for (int i = 0; i < n; i++)
        {
            arr[i] = buildings[i];
        }
    }
    else if (argc == 3)
    {
        nThreads = atoi(argv[1]);             // Get number of threads
        n = atoi(argv[2]);                    // Get array size
        arr = (int *)malloc(n * sizeof(int)); // Allocate memory to the array
        // Array "heights" will generate between 0 and 15
        for (int i = 0; i < n; i++)
        {
            arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
        }
    }
    else
    {
        Usage(argv[0]);
    }

    printf("OMP_twoptr:\n");
    printf("N size: %d\n", n);
    printf("Number of threads: %d\n", nThreads);
    start = omp_get_wtime();
    printf("Maximum trapped rainwater: %d units\n", maxWater(arr, n, nThreads));
    elapsed = omp_get_wtime() - start;
    printf("Elapsed time: %lf\n", elapsed);

    free(arr);
    return 0;
}