#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <string>
#include <fstream>

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
    // check for name
    if (string(argv[1]) == "--name") { cout << "Josh Miltier" << endl; return 0; }

    fstream file(string(argv[1]), ios_base::in);
    int arraysize = 0;
    string line;
    while (getline(file, line)) {
        arraysize++;
    }

    int array[arraysize];
    int a;
    int b = 0;
    fstream infile(string(argv[1]), ios_base::in);
    while (infile >> a) {
        array[b] = a;
        b++;
    }

    // quick sort
    if (string(argv[4]) == "--alg=quick") quickSort(array, 0, arraysize - 1);
    // merge sort
    else if (string(argv[4]) == "--alg=merge") mergeSort(array, 0, arraysize - 1, arraysize);
    // printList(array, arraysize);

    ofstream outfile;
    outfile.open(string(argv[3]));
    for (int i = 0; i < arraysize; i++) {
      outfile << array[i] << endl;
    }

    outfile.close();

    return 0;
}
