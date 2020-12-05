#include "containers.h"

using namespace std;

/* execution time struct */
typedef chrono::high_resolution_clock Clock;

	// execution start time
	auto start_time = Clock::now();
	// FUNCTION TO TIME
	// execution end time
	auto end_time = Clock::now();

/* ===================== MAIN ==================== */
int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	string inputFile = args_parsed.INPUTFILE;
	NUM_THREADS = args_parsed.NUM_THREADS;
	size_t algorithm = args_parsed.ALGORITHM;

	// read in array of integers from input file
	fstream file(inputFile.c_str(), ios_base::in);
	size_t a, b = 0;
	fstream infile(inputFile, ios_base::in);
	while (infile >> a) {
		test_vec[b++] = a;
	}