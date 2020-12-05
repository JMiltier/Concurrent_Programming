#include "containers.h"

/* ===================== MAIN ==================== */
int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	inputFile = args_parsed.INPUTFILE;
	NUM_THREADS = args_parsed.NUM_THREADS;
	algorithm = args_parsed.ALGORITHM;

	// read in integers from input file, allocate vector size, and store values
	fstream file(inputFile.c_str(), ios_base::in);
	fstream infile(inputFile, ios_base::in);
	while (getline(file, line)) s++;
	test_vec.resize(s);
	while (infile >> a) {
		test_vec[b++] = a;
	}

	// execution start time
	// auto start_time = Clock::now();
	// // FUNCTION TO TIME
	// // execution end time
	// auto end_time = Clock::now();
}