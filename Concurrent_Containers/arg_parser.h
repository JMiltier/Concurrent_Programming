#ifdef __APPLE__

#ifndef ARG_PARSER
#define ARG_PARSER

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct arg_params {
  size_t NUM_THREADS, ALGORITHM;
  string INPUTFILE;
};

struct arg_params arg_parser(int argc, const char* argv[]){
  struct arg_params args;

  /* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ ARG CHECKER ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
    if (argc == 2 && string(argv[1]) == "--name"){
      printf("Josh Miltier\n"); // check for --name
      exit(-1);
    } else if (argc == 8) {} // continue on to execution
    else { // catch all else
      printf("\nERROR: Program option parameters are invalid.\n"
             "         Expected 7 options, but got %i.\n"
             "         Refer to the README for execution instructions.\n\n", argc-1);
      exit(-1);
    }
    /* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ARG CHECK ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

    /* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ ARG PARSER ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
    // txt input file (argv 1)
    args.INPUTFILE = string(argv[1]);

    // number of threads (argv 2), with syntax checking
    if (!string(argv[2]).rfind("-t", 0)) {
      args.NUM_THREADS = atoi( argv[3] );
      if (args.NUM_THREADS > 150) { printf("ERROR; too many threads\n"); exit(-1); }
      else if (args.NUM_THREADS < 1) { printf("ERROR; not enough threads\n"); exit(-1); }
    } else {
      printf("ERROR: incorrect flag for number of threads.   Expected: '-t'  Actual: '%s'\n", argv[2]); exit(-1);
    }

    // number of iterations (argv 3), with syntax checking
    // if (string(argv[3]).rfind("-i", 0)) {
    //   printf("ERROR: incorrect flag for output file.   Expected: '-i'  Actual: '%s'\n", argv[3]); exit(-1);
    // } else {
    //   size_t pos = string(argv[3]).find("=")+1;
    //   string num = string(argv[3]).substr(pos);
    //   stringstream str2int(num);
    //   str2int >> args.NUM_ITERATIONS;
    // }

    // container type <sgl_stack,sgl_queue,treiber_stack,ms_stack> (argv 4)
    if (string(argv[4]) == "--container=sgl_stack") args.ALGORITHM= 1;
    else if (string(argv[4]) == "--container=sgl_queue") args.ALGORITHM= 2;
    else if (string(argv[4]) == "--container=treiber_stack") args.ALGORITHM = 3;
    else if (string(argv[4]) == "--container=ms_stack") args.ALGORITHM = 4;
    else if (string(argv[4]) == "--container=1") args.ALGORITHM = 5;
    else if (string(argv[4]) == "--container=2") args.ALGORITHM = 6;
    else {
      printf("ERROR: argument does not exist. Try '--container=' with an available specified method. Refer to README if needed.\n");
      exit(-1);
    }
    /* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ARG PARSER ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

    return args;
}

#endif // ARG_PARSER
#endif // __APPLE__