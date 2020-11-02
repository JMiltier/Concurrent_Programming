#ifdef __APPLE__

#ifndef ARG_PARSER
#define ARG_PARSER

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct arg_params {
  string inputFile, outputFile;
};

struct arg_params arg_parser(int argc, const char* argv[]){
  struct arg_params args;

  /* ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ ARG CHECKER ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */
    if (argc == 2 && string(argv[1]) == "--name"){
      printf("Josh Miltier\n"); // check for --name
      exit(-1);
    } else if (argc == 4) {} // continue on to execution
    else { // catch all else
      printf("\nERROR: Program option parameters are invalid.\n"
            "       Expected 3 options, but got %i.\n"
            "       Refer to the README for execution instructions.\n\n", argc-1);
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
    /* ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ END ARG PARSER ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ */

    return args;
}

#endif // ARG_PARSER
#endif // __APPLE__