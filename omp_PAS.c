/** FILE: omp_PAS.c
 *  Description: Here we are implementing the omp version of the preffix and suffix approach to the catching rain water problem.
 *          gcc -o omppas omp_PAS.c -fopenmp
 *  Execution:
 *          ./omppas <array size> <numthreads>
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
void fillLeftAndRight(int *orig , int *left, int num , int *right)  //might have to change orif to a non pointer for security 
{
    // do omp sections and have the threads at least populate left and right in parrallel 
    left[0] = orig[0];
    right[num - 1] = orig[num - 1];
    #pragma omp parrallel
    {
    #pragma omp sections nowait
    {
        #pragma omp section
        {
            for (int i = 1; i < num; i++)
            {
                if (left[i-1] > orig[i])
                {left[i] =left[i-1];}
                else {left[i]=orig[i];} 
            }
        }
    #pragma omp section 
    {
        for (int j = num - 2; j >= 0;j--)
        {
            if (right[j+1] > orig[j])
            {  right[j]=right[j+1];  }
            else {  right[j]=orig[j];  } 
        }
    }
    }
}

}


int maxWater(int arr[], int num)
{
    
    int *left;                     // setting size of left
    left= (int *)malloc(num * sizeof(int));
    int *right;                    // setting size of right
    right=(int *)malloc(num * sizeof(int));
    int res = 0;                    // amount we can have
    fillLeftAndRight(arr,left,num, right);             // Fill left adn right array
#pragma omp parallel for reduction(+:res)               // could be wrong
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
    fprintf(stderr, "\nIncorrect number of arguments:\n---- USAGE: %s <array size> <nThreads> ----\n\n", prog_name);
    exit(1);
}

int main(int argc, char *argv[])
{
    int num, nThreads;
    int *arr;
    double start, elapsed;

    if (argc != 3)
    {
        Usage(argv[0]);
    }

    num = atoi(argv[1]);      // Get array size
    nThreads = atoi(argv[2]); // Get number of threads
    omp_set_num_threads(nThreads);
    arr = (int *)malloc(num * sizeof(int));
    // Generate random array of n integers
    // srand(time(NULL));    Removed for testing consistency
    // Array "heights" will generate between 0 and 15
    for (int i = 0; i < num; i++)
    {
        arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
    }
    start = omp_get_wtime();
    //int n = sizeof(arr) / sizeof(arr[0]);
    printf(" \n");
    printf("totalwater :%d  \n", maxWater(arr, num));
    elapsed = omp_get_wtime() - start;
    printf("elapsed time :   %lf \n", elapsed);
    return 0;
}
