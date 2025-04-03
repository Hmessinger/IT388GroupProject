/** FILE: sequentialTRW.c
 *  Description: Here we are implementing the sequential version of the trapping rainwater problem. This version iterates
 *               through the array of values and it finds the highest bars on teh left and right sides. Then it will take the smaller of the two heights
 *               to find how much rainwater can be trapped in between those two bars. This approach takes a time of O(n^2).
 *  Compile:
 *          gcc -o seqNaive sequentialNaiveTRW.c
 *  Execution:
 *          ./seqNaive
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

#include <stdio.h>

// Function to return the maximum water that can be stored
int maxWater(int arr[], int n)
{
    int res = 0;

    // For every element of the array
    for (int i = 1; i < n - 1; i++)
    {

        // Find the maximum element on its left
        int left = arr[i];
        for (int j = 0; j < i; j++)
            if (arr[j] > left)
                left = arr[j];

        // Find the maximum element on its right
        int right = arr[i];
        for (int j = i + 1; j < n; j++)
            if (arr[j] > right)
                right = arr[j];

        // Update the maximum water
        res += (left < right ? left : right) - arr[i];
    }

    return res;
}

// Driver code
int main()
{
    int arr[] = {3, 1, 5, 3, 1, 0, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("%d", maxWater(arr, n));
    return 0;
}