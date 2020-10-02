// C++ program to sort an array using bucket sort
#ifdef __APPLE__

#ifndef LK_BUCKETSORT
#define LK_BUCKETSORT

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ BUCKET SORT ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
// Function to sort arr[] of size n using bucket sort
void lk_bucketSort(int *arr, int n) {
	int i, j, count = 0, max_value = 0;

	// find max key value in arr
	for (i = 0; i < n; ++i)
		if (arr[i] > max_value) max_value = arr[i];

	// create n empty buckets
	auto buckets = vector<unsigned >(static_cast<unsigned int>(max_value + 1));

	// put array elements in different buckets
	for (i = 0; i < n; ++i)
		buckets[arr[i]]++;

	// concatenate all buckets into arr[]
	for (int i = 0; i < buckets.capacity(); ++i)
		for (int j = 0; j < buckets[i]; ++j) {
				arr[count] = i;
				count++;
		}
}
/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END BUCKET SORT ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

#endif // LK_BUCKETSORT
#endif // __APPLE__