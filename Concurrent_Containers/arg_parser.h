#ifdef __APPLE__

#ifndef ARG_PARSER
#define ARG_PARSER

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void helpfn();

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
    }if (argc == 2 && string(argv[1]) == "-h"){
      helpfn(); exit(-1);
    } else if (argc == 5) {} // continue on to execution
    else { // catch all else
      printf("\nERROR: Program option parameters are invalid.\n"
             "         Expected 4 options, but got %i.\n"
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

    // container type <sgl_stack,sgl_queue,treiber_stack,ms_queue> (argv 4)
    if (string(argv[4]) == "--algorithm=sgl_stack") args.ALGORITHM= 1;
    else if (string(argv[4]) == "--algorithm=sgl_queue") args.ALGORITHM= 2;
    else if (string(argv[4]) == "--algorithm=treiber_stack") args.ALGORITHM = 3;
    else if (string(argv[4]) == "--algorithm=ms_queue") args.ALGORITHM = 4;
    else if (string(argv[4]) == "--algorithm=baskets_queue") args.ALGORITHM = 5;
    else if (string(argv[4]) == "--algorithm=elim_t_stack") args.ALGORITHM = 6;
    else if (string(argv[4]) == "--algorithm=elim_sgl_stack") args.ALGORITHM = 7;
    else {
      printf("ERROR: argument does not exist. Try '--container=' with an available specified method. Refer to README if needed.\n");
      exit(-1);
    }
    /* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ARG PARSER ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

    return args;
}

void helpfn() {
  printf("Program execution is as follows: \n"
        "  ./containers     : call to program \n"
        "  <input_file>     : list the input file with list of integers to test,     example: 'source.txt'\n"
        "  -t <NUM THREADS> : number of threads to execute the program with (1-150), example: '-t 5'\n"
        "  --algorithm=     : options available are:\n"
        "                     sgl_stack, sgl_queue, treiber_stack, ms_queue, baskets_queue, elim_sql_stack, and elim_t_stack\n"
        "                     example: '--algorithm=sgl_stack'\n\n"
        "Example with all :\n"
        "    ./container source.txt -t 5 --algorithm=sgl_stack\n\n"
        "Additionally, '--test' flag will run tests (with output) for all algorithms described above\n"
        "              '--name' flag will list out project owner's name\n\n"
        );
}

#endif // ARG_PARSER
#endif // __APPLE__