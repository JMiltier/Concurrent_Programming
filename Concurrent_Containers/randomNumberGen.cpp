#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

struct timespec time_start, time_end;

// generates file with random numbers
int main(int argc, const char* argv[]) {
   clock_gettime(CLOCK_MONOTONIC,&time_start);

   ofstream file;
   string fileOut = "source.txt";
   file.open(fileOut);
   srand((unsigned)(0));
   unsigned int size = 1e4, i, ran_data;
   if (argc == 2) size = atoi(argv[1]);
   if (size > 1e6) size = 1e6; // keep number generation to 1 million or less

   for (i = 0; i < size; i++) {
      ran_data = (rand() % (size*3)+1) + 1; // generates int between 1 - <size>
      file << ran_data << endl;
   }

   file.close();
   clock_gettime(CLOCK_MONOTONIC,&time_end);

   printf("Generated %i numbers between 1 and %i, and saved to %s\n", size, size*3, fileOut.c_str());

   unsigned long long elapsed_ns;
	elapsed_ns = (time_end.tv_sec-time_start.tv_sec)*1e9 + (time_end.tv_nsec-time_start.tv_nsec);
	printf("Elapsed (ns): %llu\n",elapsed_ns);
   double elapsed_s = ((double)elapsed_ns)/1.0e9;
	printf("Elapsed (s): %lf\n",elapsed_s);

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
