/** FILE: omp_PAS.c
 *  Description: Here we are implementing the omp version of the preffix and suffix approach to the catching rain water problem.
 *          gcc -o omppas omp_PAS.c
 *  Execution:
 *          ./omppas <array size>
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

#include<omp.h>
#include <stdio.h>