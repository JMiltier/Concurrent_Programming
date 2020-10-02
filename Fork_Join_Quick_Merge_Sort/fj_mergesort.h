#ifdef __APPLE__

#ifndef FJ_MERGESORT
#define FJ_MERGESORT

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ MERGE SORT ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
void fj_merge(int *arr, int low, int high, int mid, int len) {
	int i, j, k, c[len];
	i = low;
	k = low;
	j = mid + 1;

	while (i <= mid && j <= high) {
		if (arr[i] < arr[j]) {
			c[k] = arr[i];
			k++;
			i++;
		} else {
			c[k] = arr[j];
			k++;
			j++;
		}
	}

	while (i <= mid) {
		c[k] = arr[i];
		k++;
		i++;
	}

	while (j <= high) {
		c[k] = arr[j];
		k++;
		j++;
	}

	for (i = low; i < k; i++)
		arr[i] = c[i];
}

void fj_mergeSort(int *arr, int low, int high, int len) {
    int mid;
    if (low < high){
        //divide the array at mid and sort independently using merge sort
        mid=(low+high)/2;
        fj_mergeSort(arr,low,mid,len);
        fj_mergeSort(arr,mid+1,high,len);
        //merge or conquer sorted arrays
        fj_merge(arr,low,high,mid,len);
    }
}
/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END MERGE SORT ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

#endif // FJ_MERGESORT
#endif // __APPLE__