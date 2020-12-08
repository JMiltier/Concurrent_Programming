#include "containers.h"

/* ===================== MAIN ==================== */
int main(int argc, const char* argv[]){
	struct arg_params args_parsed = arg_parser(argc, argv);
	inputFile = args_parsed.INPUTFILE;
	NUM_THREADS = args_parsed.NUM_THREADS;
	algorithm = args_parsed.ALGORITHM;
	test = args_parsed.TEST;

	threads = static_cast<pthread_t*>(malloc(NUM_THREADS*sizeof(pthread_t)));
	pthread_mutex_init(&SGL_stack_lock, NULL);
	pthread_mutex_init(&SGL_queue_lock, NULL);

	if (inputFile != "") {
		// read in integers from input file, allocate vector size, and store values
		fstream file(inputFile.c_str(), ios_base::in);
		fstream infile(inputFile, ios_base::in);
		while (getline(file, line)) s++;
		test_vec.resize(s);
		while (infile >> a) {
			test_vec[b++] = a;
		}
	}

	// execution start time
	auto start_time = Clock::now();

	// testing flag
	if(test && algorithm == 0)
		runTests();
		exit(0);

	/* algorithm statement (from parser) is as follows:
	* 1-sgl_stack, 2-sgl_queue, 3-treiber_stack, 4-ms_queue
	* 5-baskets_queue, 6-elim_t_stack, 7-elim_sgl_stack */
	switch (algorithm) {
		// sgl_stack
		case 1:
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_create(&threads[i], NULL, SGL_stack, (void*)i);
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_join(threads[i], NULL);
			break;
		// sgl_queue
		case 2:
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_create(&threads[i], NULL, SGL_queue, (void*)i);
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_join(threads[i], NULL);
			break;
		// treiber_stack
		case 3:
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_create(&threads[i], NULL, treiber_stack, (void*)i);
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_join(threads[i], NULL);
			break;
		// MS_queue
		case 4:
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_create(&threads[i], NULL, MS_queue, (void*)i);
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_join(threads[i], NULL);
			break;
		// baskets_queue
		case 5:
			init_queue(bas_queue);
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_create(&threads[i], NULL, baskets_queue, (void*)i);
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_join(threads[i], NULL);
			break;
		// elim_sql_stack
		case 6:
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_create(&threads[i], NULL, elim_sgl_stack, (void*)i);
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_join(threads[i], NULL);
			break;
		// elim_t_stack
		case 7:
			for (size_t i = 0; i < NUM_THREADS; i++)
				pthread_create(&threads[i], NULL, elim_t_stack, (void*)i);
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
	return 0;
}