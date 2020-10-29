#ifdef __APPLE__

#ifndef ARG_PARSER
#define ARG_PARSER

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct arg_params {
  string inputFile, outputFile, algorithm;
  size_t NUM_THREADS;
  int bar, lock;
};

struct arg_params arg_parser(int argc, const char* argv[]){
  struct arg_params args;

  /* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ ARG CHECKER ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
    if (argc == 2 && string(argv[1]) == "--name"){
      printf("Josh Miltier\n"); // check for --name
      exit(-1);
    } else if (argc == 9) {} // continue on to execution
    else { // catch all else
      printf("\nERROR: Program option parameters are invalid.\n"
             "         Expected 8 options, but got %i.\n"
             "         Refer to the README for execution instructions.\n\n", argc-1);
      exit(-1);
    }
    /* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ARG CHECK ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

    /* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ ARG PARSER ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
    // txt input file (argv 1)
    args.inputFile = string(argv[1]);

    // txt output file (argv 3), with syntax checking
    if (string(argv[2]).rfind("-o", 0)) {
      printf("ERROR: incorrect flag for output file.   Expected: '-o'  Actual: '%s'\n", argv[2]); exit(-1);
    } else args.outputFile = string(argv[3]);

    // number of threads (argv 5), with syntax checking
    if (!string(argv[4]).rfind("-t", 0)) {
      args.NUM_THREADS = atoi( argv[5] );
      if (args.NUM_THREADS > 150) { printf("ERROR; too many threads\n"); exit(-1); }
    } else {
      printf("ERROR: incorrect flag for number of threads.   Expected: '-t'  Actual: '%s'\n", argv[4]); exit(-1);
    }

    // algorithm type <lkbucket> (argv 6)
    if (string(argv[6]) == "--alg=lkbucket") args.algorithm = "lkbucket";
    // algorithm not available
    else { printf("ERROR: algorithm does not exist. Try 'fjmerge' or 'lkbucket'\n"); exit(-1); }

    // bar type <sense> or <pthread> (argv 4)
    if (string(argv[7]) == "--bar=sense") args.bar= 1;
    else if (string(argv[7]) == "--bar=pthread") args.bar= 2;
    else {
      printf("ERROR: argument does not exist. Try '--bar=' with an available specified method. Refer to README if needed.\n");
      exit(-1);
    }

    // lock type <tas, ttas, ticket, mcs, pthread> (argv 4)
    if (string(argv[8]) == "--lock=tas") args.lock = 1;
    else if (string(argv[8]) == "--lock=ttas") args.lock = 2;
    else if (string(argv[8]) == "--lock=ticket") args.lock = 3;
    else if (string(argv[8]) == "--lock=pthread") args.lock = 4;
    else {
      printf("ERROR: argument does not exist. Try '--lock=' with an available specified method. Refer to README if needed.\n");
      exit(-1);
    }
    /* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ARG PARSER ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

    return args;
}

#endif // ARG_PARSER
#endif // __APPLE__