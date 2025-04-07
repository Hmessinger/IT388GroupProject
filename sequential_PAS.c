/** FILE: sequencial_PAS.c
 *  Description: Here we are implenting the sequencial code of catching rain water.this version we calulcalute the highest bar on the left first and on the right. we use pointers
 * to traverse the arra. 
 *  Compile:
 *          gcc -o spas sequential_PAS.c
 *  Execution:
 *          ./spas <array size>     implemented later
 *
 * Name: Hogan Messinger, Suhail Tailor, Derek Kmieciak, Angel Hernandez
 * Course: IT 388-Spring 2025
 * Group Project
 */

#include <stdio.h>

int maxWater(int arr[], int n) {

    // Left[i] contains height of tallest bar to the
    // left of i'th bar including itself
    int left[n];

    // Right[i] contains height of tallest bar to
    // the right of i'th bar including itself
    int right[n];

    int res = 0;

    // Fill left array
    left[0] = arr[0];
    for (int i = 1; i < n; i++)
        left[i] = left[i - 1] > arr[i] ? left[i - 1] : arr[i];

    // Fill right array
    right[n - 1] = arr[n - 1];
    for (int i = n - 2; i >= 0; i--)
        right[i] = right[i + 1] > arr[i] ? right[i + 1] : arr[i];

    // Calculate the accumulated water element by element
    for (int i = 1; i < n - 1; i++) {
        int minOf2 = left[i - 1] < right[i + 1] ? left[i - 1] : right[i + 1];
        if (minOf2 > arr[i]) {
            res += minOf2 - arr[i];
        }
    }

    return res;
}

int main() {
    int arr[] = { 2, 1, 5, 3, 1, 0, 4 };
    int n = sizeof(arr) / sizeof(arr[0]);
    printf("%d", maxWater(arr, n));
    return 0;
}
