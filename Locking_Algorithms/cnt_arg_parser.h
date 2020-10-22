#ifdef __APPLE__

#ifndef CNT_ARG_PARSER
#define CNT_ARG_PARSER

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct arg_params {
  string outputFile;
  int argument;
  size_t NUM_THREADS, NUM_ITERATIONS;
};

struct arg_params arg_parser(int argc, const char* argv[]){
  struct arg_params args;

  /* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ ARG CHECKER ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
  if (argc == 2 && string(argv[1]) == "--name") {
    // check for --name
    printf("Josh Miltier\n");
    exit(0);
  }
  else if (argc == 7) {} // continue on to execution
  else { // catch all else
    printf("\nERROR: Program option parameters are invalid.\n"
          "       Expected 6 options, but got %i.\n"
          "       Refer to the README for execution instructions.\n\n", argc-1);
    exit(-1);
  }
  /* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ARG CHECK ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

  /* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ ARG PARSER ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
  // number of threads (argv 2), with syntax checking
  if (!string(argv[1]).rfind("-t", 0)) {
    args.NUM_THREADS = atoi( argv[2] );
    if (args.NUM_THREADS > 150) { printf("ERROR; too many threads\n"); exit(-1); }
  } else {
    printf("ERROR: incorrect flag for number of threads.   Expected: '-t'  Actual: '%s'\n", argv[1]); exit(-1);
  }

  // number of iterations (argv 3), with syntax checking
  if (string(argv[3]).rfind("-i", 0)) {
    printf("ERROR: incorrect flag for output file.   Expected: '-i'  Actual: '%s'\n", argv[2]); exit(-1);
  } else {
    size_t pos = string(argv[3]).find("=")+1;
    string num = string(argv[3]).substr(pos);
    stringstream str2int(num);
    str2int >> args.NUM_ITERATIONS;
  }
  // bar type <sense> or <pthread> (argv 4)
  if (string(argv[4]) == "--bar=sense") args.argument= 1;
  else if (string(argv[4]) == "--bar=pthread") args.argument= 2;

  // lock type <tas, ttas, ticket, mcs, pthread> (argv 4)
  else if (string(argv[4]) == "--lock=tas") args.argument = 3;
  else if (string(argv[4]) == "--lock=ttas") args.argument = 4;
  else if (string(argv[4]) == "--lock=ticket") args.argument = 5;
  else if (string(argv[4]) == "--lock=pthread") args.argument = 6;
  else { printf("ERROR: argument does not exist. Try '--bar=' or '--lock=' with an available specified method. Refer to README if needed.\n"); exit(-1); }

  // txt output file (argv 6), with syntax checking
  if (string(argv[5]).rfind("-o", 0)) {
    printf("ERROR: incorrect flag for output file.   Expected: '-o'  Actual: '%s'\n", argv[5]); exit(-1);
  } else args.outputFile = string(argv[6]);

/* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ARG PARSER ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

  return args;
}

#endif // CNT_ARG_PARSER
#endif // __APPLE__