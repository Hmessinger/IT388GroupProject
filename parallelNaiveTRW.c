/** FILE: parallelTRW.c
 *  Description: Here we are adding openMP to the sequential (naive) version of the trapping rainwater problem. This will parallelize the function that
 *               calculates the amount of rainwater that can be trapped. This approach will speedup the time complexity because there will be multiple
 *               threads working on the computations.
 *  Compile:
 *          gcc -o parNaive parallelNaiveTRW.c
 *  Execution:
 *          ./parNaive
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */
