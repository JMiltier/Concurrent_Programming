#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <quicksort.cpp>
#include <mergesort.cpp>

using namespace std;

//global counter
static unsigned long int swapCount = 0;

//check if array sorted correctly - by comparing two sorted arrays
void arrayCheck(int arraysize, int *array1, int *array2){
    for(int i=0;i<arraysize;i++){
        if(array1[i]!=array2[i]){
            cout<<"Error with the previous sortings!\n\n";
            return;
        }
    }
}

int main(){
    const int arraysize = 500000;
    cout<<"Using array size of "<<arraysize<<endl<<endl;

    //dynamically allocate arrays
    int *array1, *array2, *array3;
    array1 = new int [arraysize];
    array2 = new int [arraysize];

    for(unsigned long int i=0;i<arraysize;i++){
        array1[i]=rand();
        array2[i]=array1[i]; //=array1[i]; //to sure all of the arrays are the same (FAIRNESS!)
    }

    time_t begin, end; // time_t is a datatype to store time values.

    // quick sort
    time (&begin);
    quickSort(array1,0,arraysize-1);
    time (&end);
    long double timediff = difftime (end,begin);
    cout<<"Array sorted after "<<swapCount<<" swaps using the ''Quick Sort'' method."<<endl;
    cout<<"Run time: "<<timediff<<" seconds"<<endl<<endl;

    // merge sort
    swapCount = 0; //reset global variable to 0
    time (&begin);
    mergeSort(array2, 0, arraysize);
    time (&end);
    cout<<"Array sorted after "<<swapCount<<" swaps using the ''Shell Sort'' method."<<endl;
    cout<<"Run time: "<<difftime (end,begin)<<" seconds"<<endl<<endl;

    arrayCheck(arraysize, array1, array2);

  // clean-up
  delete [] array1; delete [] array2;

  return 0;
}
