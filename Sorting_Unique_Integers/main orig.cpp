#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <string>
#include "quicksort.h"
#include "mergesort.h"

using namespace std;

//check if array sorted correctly - by comparing two sorted arrays
void arrayCheck(int arraysize, int *array1, int *array2) {
  for (int i = 0; i < arraysize; i++) {
    if (array1[i] != array2[i]) {
        cout << "Error with the previous sortings!\n\n";
        return;
    }
  }
}

//debugging purposes
void printList(int arr[], int length){
    for (int i = 0; i<length; i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

// Merge sort
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
    for (i = low; i < k; i++)  {
        arr[i] = c[i];
    }
}

void mergeSort(int *arr, int low, int high, int len) {
    int mid;
    if (low < high){
        //divide the array at mid and sort independently using merge sort
        mid=(low+high)/2;
        mergeSort(arr,low,mid,len);
        mergeSort(arr,mid+1,high,len);
        //merge or conquer sorted arrays
        merge(arr,low,high,mid,len);
    }
}

//quick sort
void quickSort(int *arr, int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = arr[(left + right) / 2];

    /* partition */
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
            if (i <= j) {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                i++;
                j--;
            }
    };

    /* recursion */
    if (left < j){
        quickSort(arr, left, j);
    }if (i < right){
        quickSort(arr, i, right);
    }
}

int main(int argc, char *argv[]) {
    // switch (getopt(argc, argv, "nt:")) {
    //     case '--name': // check for name
    //         cout << "Josh Miltier" << endl;
    //         return 0;
    // }

    // check for name
    if (string(argv[1]) == "--name") { cout << "Josh Miltier" << endl; return 0; }

    const int arraysize = 100;
    // cout << "Using array size of " << arraysize << endl
    //     << endl;

    //dynamically allocate arrays
    int *array1, *array2;
    array1 = new int[arraysize];
    array2 = new int[arraysize];

    for (int i = 0; i < arraysize; i++) {
        array1[i] = rand() % 1000 + 1;
        array2[i] = array1[i]; //=array1[i]; //to sure all of the arrays are the same (FAIRNESS!)
    }

//   printList(array1, arraysize);

    time_t begin, end; // time_t is a datatype to store time values.

    // quick sort
    time(&begin);
    quickSort(array1, 0, arraysize - 1);
    time(&end);
    double timediff = difftime(end, begin);
    cout << "Run time: " << timediff << " seconds" << endl
        << endl;

    // merge sort
    time(&begin);
    mergeSort(array2, 0, arraysize - 1, arraysize);
    time(&end);
    cout << "Run time: " << difftime(end, begin) << " seconds" << endl
        << endl;

    arrayCheck(arraysize, array1, array2);
    // printList(array1, arraysize);

    // clean-up
    delete[] array1;
    delete[] array2;

    return 0;
}
