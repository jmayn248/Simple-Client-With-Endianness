//Citations
//1. parse.c
//Author: Dr. Dylan Schwesinger
//Retrieval Date: November 6, 2024
//Source: Dr. Schwesinger's public directory on acad.kutztown.edu
//Note: used parse.c as a foundation for parse.cpp

#include <unistd.h>

#include "parse.h"

using namespace std;

struct options parse(int argc, char** argv) {
  struct options o;
  int opt;

  //need to initialize members of options struct to default values
  o.host = "";
  o.port = "";
  o.numStr = 1;
  o.h_flag = false;
  o.p_flag = false;

  while ((opt = getopt(argc, argv, "h:p:n:")) != -1) {
    switch(opt)
      {
      case 'h':
        o.host = optarg;
        o.h_flag = true;
        break;
      case 'p':
        o.port = optarg;
        o.p_flag = true;
        break;
      case 'n':
        o.numStr = stoi(optarg);
        break;
      } //end of switch
  } //end of while loop
  return o;
} //end of parse()
