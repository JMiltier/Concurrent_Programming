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
