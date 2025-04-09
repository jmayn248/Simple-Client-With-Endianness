#ifndef PARSE_H
#define PARSE_H

#include <string>

using namespace std;

struct options {
  string host; //host address
  string port; //port number
  uint32_t numStr; //number of strings requested (default = 1)
  bool h_flag;
  bool p_flag;
};

//
// Function name: parse
// Description:
// Parses command line arguments from the user
// Parameters
// -int argc: number of arguments to be passed
// -char**: pointer to char pointer, which points to
// arguments passed
// Return Value: struct options - settings parsed from
// command line arguments
//
struct options parse(int argc, char** argv);
#endif
