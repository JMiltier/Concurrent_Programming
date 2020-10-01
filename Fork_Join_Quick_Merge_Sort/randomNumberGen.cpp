#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

// generates file with random numbers
int main(int argc, const char* argv[]) {
   ofstream file;
   string fileOut = "source.txt";
   file.open(fileOut);
   srand((unsigned)(0));
   int size = 10000, i, ran_data;
   if (argc == 2) size = atoi(argv[1]);

   for (i = 0; i < size; i++) {
      ran_data = (rand() % (size*3)+1) + 1; // generates int between 1 - <size>
      file << ran_data << endl;
   }

   file.close();
   printf("Generated %i numbers between 1 and %i, and saved to %s\n", size, size*3, fileOut.c_str());
   return 0;
}

/*
To run:
   EITHER:
      1. `makefile numGen`
   OR
      2. compile: g++ -o numGen randomNumberGen.cpp
         then execute: ./numGen <amount of numbers>
*/
