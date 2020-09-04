#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

//quick sort
void quickSort(unsigned long long int *arr, unsigned long long int left, unsigned long long int right) {
    unsigned long long int i = left, j = right;
    unsigned long long int tmp;
    unsigned long long int pivot = arr[(left + right) / 2];

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