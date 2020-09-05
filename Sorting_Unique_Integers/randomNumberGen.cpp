#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>

using namespace std;

// generates
int main() {
   ofstream file;
   file.open("sourcefile.txt");
   srand((unsigned)(0));
   int size = 1000;

   for (int i = 0; i < size; i++) {
      int ran_data = (rand() % size+1) + 1; // generates int between 1 - <size>
      file << ran_data << endl;
   }

   file.close();
}
