/*
OMP only approach to the trapping rainwater problem.
Naive approach: O(n^2) double loop

Code credit: Geeks For Geeks
    https://geeksforgeeks.org/trapping-rain-water/#

This code will be modified into the parallel final code. 
This is the first modification of the the sequential code. It uses only OMP to solve the problem. 

Compile: gcc -o omp_naive omp_naive.c -fopenmp
Execute: ./omp_naive <array size> <nThreads>
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h> //"Inclue errors detected" but then it works fine ok vro :broken_heart:

int maxWater(int arr[], int n, int nThreads)
{
    int result = 0;
    //TODO:
    //This is the less efficient of the solutions. More efficienct would be to put j on the outside, but not sure how to do that w/ 2 inner loops
    #pragma omp parallel for num_threads(nThreads) reduction(+:result)
    for(int i=1; i<n-1; i++)
    {
        //Find max to the left
        int left = arr[i];
        for(int j=0; j<i; j++)
        {
            if(arr[j]>left)
                left = arr[j];
        }
        //Find max to the right
        int right = 0; 
        for(int j=i+1; j<n; j++)
        {
            if(arr[j]>right)
                right = arr[j];
        }
        //Update max water result
        result += (left < right ? left : right ) - arr[i];
    }
    
    return result;
}

int main(int argc, char *argv[])
{
    int n;
    int nThreads;
    int *arr;
    double start, end;

    if(argc !=3 )
    {
        fprintf(stderr, "\nIncorrect number of arguments\n\t---USAGE: ./omp_naive <array size> <nThreads>\n\n");
        exit(1);
    }
    n = atoi(argv[1]); //Get array size
    nThreads = atoi(argv[2]); //Get number of threads
    arr = (int*)malloc(n*sizeof(int));
    //Generate random array of n integers
    //Array "heights" will generate between 0 and 15
    for(int i=0; i<n; i++)
    {
        //TODO: "Random" array is generated the name for same n every time
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
    start = omp_get_wtime();
    printf("\n---------------------------------------\n");
    printf("Maximum trapped rainwater: %d units\n", maxWater(arr, n, nThreads));
    end = omp_get_wtime() - start;
    printf("Elapsed time: %lf\n", end);

    free(arr);
    return 0;
}