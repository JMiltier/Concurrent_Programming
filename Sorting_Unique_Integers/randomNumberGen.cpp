#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace std;

int main() {
  srand(time(NULL));
  int value = 0;
  ofstream myfile ("list.txt");

  myfile.open ("list.txt", ios::out | ios::app );

  if (myfile.is_open()) {
    for (int i = 0; i < 1000; ++i) {
      value = rand();
      myfile << i << " " << value << "\n";
    }
    myfile.close();
  } else cout << "Unable to open file";

  return 0;
}
