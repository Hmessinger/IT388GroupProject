/*
Sequential approach to the trapping rainwater problem.
Naive approach: O(n^2) double loop

Code credit: Geeks For Geeks
    https://geeksforgeeks.org/trapping-rain-water/#

This code will be modified into the parallel final code. 

Compile: gcc -o seq_naive sequential_naive.c
Execute: ./seq_naive <array size>
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int n;
    int *arr;

    if(argc !=2 )
    {
        fprintf(stderr, "\nIncorrect number of arguments\n\t---USAGE: ./seq_naive <array size>\n\n");
        exit(1);
    }
    n = atoi(argv[1]); //Get array size
    arr = (int*)malloc(n*sizeof(int));
    //Generate random array of n integers
    //Array "heights" will generate between 0 and 15
    for(int i=0; i<n; i++)
    {
        //TODO: "Random" array is generated the name for same n every time
        arr[i] = (int)((double)rand() / ((double)RAND_MAX + 1) * 16);
    }
    
    //Working code here...
    printf("TEST:\n");
    for(int i=0; i<n; i++)
    {
        printf("[%d] ",arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}