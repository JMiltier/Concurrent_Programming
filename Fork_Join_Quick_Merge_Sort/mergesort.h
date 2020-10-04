#ifdef __APPLE__

#ifndef MERGESORT
#define MERGESORT

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ MERGE SORT ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
void merge(int *arr, int low, int high, int mid) {
	int l_size = mid - low + 1, r_size = high - mid, i, j, k;
	int* left = new int[l_size];
	int* right = new int[r_size];

	for (i = 0; i < l_size; i++)
		left[i] = arr[i + low];

	for (i = 0; i < r_size; i++)
		right[i] = arr[i + mid + 1];

	i = j = 0; k = low;

	while(i < l_size && j < r_size){
		if (left[i] <= right[j]) arr[k++] = left[i++];
		else arr[k++] = right[j++];
	}

	while (i < l_size) arr[k++] = left[i++];
	while (j < r_size) arr[k++] = right[j++];
}

void mergeSort(int *arr, int low, int high) {
    //divide the array at mid
		int mid = low + (high - low) / 2;
    if (low < high){
        // sort independently using merge sort
        mid=(low+high)/2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        //merge or conquer sorted arrays
        merge(arr, low, high, mid);
    }
}
/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END MERGE SORT ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

#endif // MERGESORT
#endif // __APPLE__