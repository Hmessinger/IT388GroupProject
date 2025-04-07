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
 #include<stdlib.h>
 #include <stdio.h>


int main(int argc, char *argv[])
{
    int n, rank, size;
    int *arr = NULL;
     double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    if (argc != 2)
    {
        if (rank == 0)
            fprintf(stderr, "Usage: %s <array size>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    n = atoi(argv[1]);

    if (rank == 0)
    {
        // generates the array
        arr = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
        {
            arr[i] = rand() % 16;
        }

        // Print array size less than 30
        if (n < 30)
        {
            printf("Array:\n");
            for (int i = 0; i < n; i++)
                printf("[%d] ", arr[i]);
            printf("\n");
        }
    }

    // Broadcast array to all processes
    if (rank != 0)
        arr = (int *)malloc(n * sizeof(int));

    MPI_Bcast(arr, n, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD); // sync all processes
    start_time = MPI_Wtime();

    // Each process 
    int local_result = 0;
    for (int i = rank + 1; i < n - 1; i += size)
    {
        int left = arr[i];
        for (int j = 0; j < i; j++)
        {
            if (arr[j] > left)
                left = arr[j];
        }

        int right = arr[i];
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] > right)
                right = arr[j];
        }

        int trapped = (left < right ? left : right) - arr[i];
        if (trapped > 0)
            local_result += trapped;
    }

    // Reduce all local results to the final answer 
    int global_result = 0;
    MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD); // sync again
    end_time = MPI_Wtime();

    if (rank == 0)
    {
        printf("---------------------------------------\n");
        printf("Maximum trapped rainwater: %d units\n", global_result);
         printf("Elapsed Time: %f seconds\n", end_time - start_time);
    }

    free(arr);
    MPI_Finalize();
    return 0;
}
