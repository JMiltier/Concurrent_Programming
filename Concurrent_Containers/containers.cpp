#include "containers.h"

/* ===================== MAIN ==================== */
int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	inputFile = args_parsed.INPUTFILE;
	NUM_THREADS = args_parsed.NUM_THREADS;
	algorithm = args_parsed.ALGORITHM;

	threads = static_cast<pthread_t*>(malloc(NUM_THREADS*sizeof(pthread_t)));
	pthread_mutex_init(&SGL_stack_lock, NULL);

	// read in integers from input file, allocate vector size, and store values
	fstream file(inputFile.c_str(), ios_base::in);
	fstream infile(inputFile, ios_base::in);
	while (getline(file, line)) s++;
	test_vec.resize(s);
	while (infile >> a) {
		test_vec[b++] = a;
	}

	// execution start time
	auto start_time = Clock::now();

	sgl_stack->size();

	/* algorithm statement (from parser) is as follows:
	* 1-sgl_stack, 2-sgl_queue */
	switch (algorithm) {
		// sgl_stack
		case 1:
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_create(&threads[i], NULL, SGL_stack, (void*)i);
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_join(threads[i], NULL);
			break;
		// something didn't match up
		default:
			printf("An error occured in main argument switch\n");
			exit(-1);
	}

	// execution end time
	auto end_time = Clock::now();
	// unsigned int 4,294,967,295, which is only 4.3 seconds
	// unsigned long, plan on never running out (over 5 centuries)
	unsigned long time_spent = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
	// printf("Time elapsed is %lu nanoseconds\n", time_spent);
	// printf("                %f seconds\n", time_spent/1e9);
}

