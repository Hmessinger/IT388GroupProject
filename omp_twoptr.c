/** FILE: omp_twoptr.c
 *  Description: Here we are implementing the OMP version of the trapping rainwater problem. 
 *  Compile:
 *          gcc -o omp_tp omp_twoptr.c -fopenmp -lm 
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
#include <math.h> //fmax function. Removes a lot of if-else statements

int maxWater(int arr[], int n, int nThreads)
{ 
    if(n<=2)
        return 0; //Can't trap any water with just 2 or fewer walls
    //The two pointer approach only works in sequence.
    //This approach divides the sequential work among multiple threads.
    int result = 0;
    int right, left, rMax, lMax, rank;
    int* local_lMax = (int*)malloc(sizeof(int) * nThreads);
    int* local_rMax = (int*)malloc(sizeof(int) * nThreads);
    int* lBounds = (int*)malloc(sizeof(int) * nThreads);
    int* rBounds = (int*)malloc(sizeof(int) * nThreads);
    int work = (n + nThreads - 1) / nThreads;

    //Find the local maxima for each of the parts
    #pragma omp parallel num_threads(nThreads) private(left, right, rank)
    {
        rank = omp_get_thread_num();
        left = rank*work;
        right = (left + work > n ? n : (left+work));
        local_lMax[rank] = arr[left];
        for(int i=left+1; i<right; i++)
            local_lMax[rank] = fmax( local_lMax[rank], arr[i] );
        local_rMax[rank] = arr[right-1];
        for(int i=right-2; i>=left; i--)
            local_rMax[rank] = fmax( local_rMax[rank], arr[i] );
    }
    //Find depedencies on borders (what if the first block in the next section would trap rainwater in this one? 
    //This prevents edges of chunks from losing water)
    //These dependencies are the reason two pointer can't be fully parallelized in the first place --
    //And the restructured version to remove the dependencies is the PAS method.
    lBounds[0] = local_lMax[0];
    for(int i=1; i<nThreads; i++)
        lBounds[i] = fmax( lBounds[i-1], local_lMax[i] );
    rBounds[nThreads-1] = local_rMax[nThreads-1];
    for(int i=nThreads-2; i>=0; i--)
        rBounds[i] = fmax( rBounds[i+1], local_rMax[i] );
    //Now two pointer approach can be used on each of the small sections without dependency issues
    //Each will be operated on by a thread - sequential work divided among parallel threads
    #pragma omp parallel num_threads(nThreads) private(left, right, rank, rMax, lMax) reduction(+:result)
    {
        rank = omp_get_thread_num();
        int start = rank * work;
        int end = (start + work > n ? n : (start + work));
        left = start;
        right = end - 1;
        lMax = arr[left];
        rMax = arr[right];

        lMax = fmax(lMax, rank > 0 ? lBounds[rank-1] : lMax);
        rMax = fmax(rMax, rank < nThreads-1 ? rBounds[rank+1] : rMax);
        while(left <= right)
        {
            if(rMax <= lMax)
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
        }
    }
    free(local_lMax);
    free(local_rMax);
    free(lBounds);
    free(rBounds);
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