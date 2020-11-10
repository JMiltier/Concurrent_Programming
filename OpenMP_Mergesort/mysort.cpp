#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <algorithm>
#include <string>
#include <omp.h>
#include "arg_parser.h"

using namespace std;

/* execution time struct */
typedef chrono::high_resolution_clock Clock;

/* merge sort functions */
void mergeOpenMP(int *arr, int low, int high, int mid, int len);
void mergeSortOpenMP(int *arr, int low, int high, int len);
void merge(int *arr, int low, int high, int mid, int len);
void mergeSort(int *arr, int low, int high, int len);
/* array sort check */
void arrayCheck(int asize, int *a1, int *a2);

/* =============================================== */
/* ===================== MAIN ==================== */
int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	string inputFile = args_parsed.inputFile;
	string outputFile = args_parsed.outputFile;

	// create array from input file
	fstream file(inputFile.c_str(), ios_base::in);
	int a, b = 0;
	int arrsize = 0;
	string line;
	while (getline(file, line)) arrsize++;
	int arr[arrsize], arr2[arrsize], arrCheck[arrsize];
	fstream infile(inputFile, ios_base::in);
	while (infile >> a) {
		arr[b] = arr2[b] = arrCheck[b]= a;
		b++;
	}

	// execution start time
	auto start_time = Clock::now();
	// OpenMP mergesort
	mergeSortOpenMP(arr, 0, arrsize - 1, arrsize);
	// execution end time
	auto end_time = Clock::now();


  /* The follow is setup for execution time comparison */
  // // execution start time
	// auto start_time2 = Clock::now();
	// // mergesort
	// mergeSort(arr2, 0, arrsize - 1, arrsize);
	// // execution end time
	// auto end_time2 = Clock::now();

	// calculate and display execution time
	unsigned long time_spent = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
	printf("Time elapsed: %lu nanoseconds\n", time_spent);
	printf("              %f seconds\n", time_spent/1e9);


  /* Determine which sorting algo is faster */
  // unsigned long time_spent2 = chrono::duration_cast<chrono::nanoseconds>(end_time2 - start_time2).count();
  // printf("              %f seconds\n", time_spent2/1e9);

  // float time_diff = float(time_spent) / float(time_spent2);
  // if (time_diff < 1) printf("OpenMP is %f times faster than normal mergesort.\n", abs(1-time_diff) + 1);
  // else printf("Normal mergesort is %f times faster than OpenMP.\n", time_diff);

	/* WRITE SORTED ARRAY TO FILE */
	ofstream outfile;
	outfile.open(outputFile);
	for (int i = 0; i < arrsize; i++) outfile << arr[i] << endl;
	outfile.close();

  // ensure array was sorted
	sort(arrCheck, arrCheck + arrsize);
	arrayCheck(arrsize, arr, arrCheck);
}

/* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ MERGE SORT ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
void mergeOpenMP(int *arr, int low, int high, int mid, int len) {
  int i, j, k, c[len];
  i = low;
  k = low;
  j = mid + 1;

  // attempt to parellize this block of code {}
	#pragma omp parallel
  {
    // omp_get_thread_num();
    // single thread, since reliant on ordering/incrementing
    #pragma omp single
    {
      while (i <= mid && j <= high) {
        if (arr[i] < arr[j]) {
          c[k] = arr[i];
          i++;
        } else  {
          c[k] = arr[j];
          j++;
        }
        k++;
      }
    }

    // single thread, since reliant on ordering/incrementing
		#pragma omp single
    {
      while (i <= mid) {
        c[k] = arr[i];
        k++;
        i++;
      }
    }

    // single thread, since reliant on ordering/incrementing
		#pragma omp single
    {
      while (j <= high) {
        c[k] = arr[j];
        k++;
        j++;
      }
    }

		#pragma omp for
      for (i = low; i < k; i++) {
        arr[i] = c[i];
      }
  } // end omp parallel
}

void mergeSortOpenMP(int *arr, int low, int high, int len) {
  if (low < high){
    //divide the array at mid and sort independently using merge sort
    int mid=(low+high)/2;
    // split in two sections
    #pragma omp parallel sections
    {
      #pragma omp section
      mergeSortOpenMP(arr,low,mid,len);
      #pragma omp section
      mergeSortOpenMP(arr,mid+1,high,len);
    }
      //merge or conquer sorted arrays
      mergeOpenMP(arr,low,high,mid,len);
  }
}
// Merge sort, from lab0
void merge(int *arr, int low, int high, int mid, int len) {
  int i, j, k, c[len];
  i = low;
  k = low;
  j = mid + 1;

  while (i <= mid && j <= high) {
      if (arr[i] < arr[j]) {
          c[k] = arr[i];
          k++;
          i++;
      }
      else  {
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

  for (i = low; i < k; i++) {
    arr[i] = c[i];
  }
}

void mergeSort(int *arr, int low, int high, int len) {
  if (low < high){
    //divide the array at mid and sort independently using merge sort
    int mid=(low+high)/2;
    mergeSort(arr,low,mid,len);
    mergeSort(arr,mid+1,high,len);
    //merge or conquer sorted arrays
    merge(arr,low,high,mid,len);
  }
}
/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END MERGE SORT ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

//check if array sorted correctly - by comparing two sorted arrays
void arrayCheck(int asize, int *a1, int *a2) {
  for (int i = 0; i < asize; i++) {
    if (a1[i] != a2[i]) {
      printf("Array is incorrectly sorted!\n%i != %i at position %i\n", a1[i], a2[i], i);
      return;
    }
  }
}
