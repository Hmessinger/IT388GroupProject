/** FILE: omp_twoptr.c
 *  Description: Here we are implementing the omp version of the two pointer approach to the catching rain water problem.
 *          gcc -o omptwoptr omp_twoptr.c
 *  Execution:
 *          ./omptwoptr <array size>
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

#include<omp.h>
#include <stdio.h>