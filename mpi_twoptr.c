/** FILE: mpi_twoptr.c
 *  Description: Here we are implementing the MPI version of the two pointer approach to the catching rain water problem.
 *          gcc -o mpitwoptr mpi_twoptr.c
 *  Execution:
 *          ./mpitwoptr <array size>
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

/*
MPI approach to the trapping rainwater problem.
Two Pointer Approach: O(n)

Code credit: Geeks For Geeks
    https://geeksforgeeks.org/trapping-rain-water/#

This code will be modified into the parallel final code.

*/


#include<mpi.h>
#include <stdio.h>