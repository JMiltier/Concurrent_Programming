#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>

using namespace std;

int main() {
   ofstream file;
   file.open("sourcefile.txt");
   srand((unsigned)(0));
   vector<int> arr;

   for (int index = 0; index < 100; index++) {
      int ran_data = (rand() % 101) + 1;
      arr.push_back(ran_data);
   }

   file << arr;
   file.close();
}
