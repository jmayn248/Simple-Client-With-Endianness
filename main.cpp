//Citations
//1. htonl()
//Source: Linux Programmer's Manual (man 3 byteorder)
//Retrieval Date: November 7, 2024
//2. shutdown(), SHUT_WR
//Source: Linux Programmer's Manual (man 2 shutdown)
//Retrieval Date: November 8, 2024

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>

#include "utility.h"
#include "parse.h"

using namespace std;

int main(int argc, char **argv) {
  struct options o; //cmd line args
  int sockfd = -1; //socket file descriptor
  int status; //return value of func - used for error checking

  //PARSE
  o = parse(argc, argv);
  if (o.h_flag == false) {
    cout << "Error: -h is a required command line argument" << endl;
  }
  if (o.p_flag == false) {
    cout << "Error: -p is a required command line argument" << endl;
  }
  if ( (o.h_flag == false) || (o.p_flag == false) ) {
    exit(EXIT_FAILURE);
  }
  
  //1. Client connects to server
  //create_and_connect_socket(): call
  sockfd = create_and_connect_socket(o.host, o.port);
  //create_and_connect_socket(): check for errors
  if (sockfd == -1)
    {
      //terminate program on failure
      exit(EXIT_FAILURE);
    }
  
  //2. Client receives string packet with string "BEGIN"
  string begin;
  //get_string_from_packet: call()
  status = get_string_from_packet(sockfd, begin);
  //get_string_from_packet: check for errors
  if (status == -1)
    {
      //1. print error message
      cout << "Error: get_string_from_packet()" << endl;
      //2. properly clean up
      close(sockfd);
      //3. terminate program on failure
      exit(EXIT_FAILURE);
    }
  cout << "Received " << '\"' << begin << '\"' << endl;

  //3. Client sends 4-byte integer in Big Endian (indicates how many random strings to receive)
  cout << "Sending " << o.numStr << endl;
  //convert o.numStr to big endian
  uint32_t numStrNetworkOrder = htonl(o.numStr);
  //send 4 byte integer
  //sendall(): call
  status = sendall(sockfd, numStrNetworkOrder);
  //sendall(): check for errors
  if (status == -1)
    {
      //1. print error message
      cout << "Error: sendall()" << endl;
      //2. properly clean up
      close(sockfd);
      //3. terminate program on failure
      exit(EXIT_FAILURE);
    }

  //4. Client receives requested number of random string packets
  string str_to_print;
  for (int i = 0; i < o.numStr; i++)
    {
      //get_string_from_packet(): call
      status = get_string_from_packet(sockfd, str_to_print);
      //get_string_from_packet(): check for errors
      if (status == -1)
        {
          //1. print error message
          cout << "Error: get_string_from_packet()" << endl;
          //2. properly clean up
          close(sockfd);
          //3. terminate program on failure
          exit(EXIT_FAILURE);
        }
      cout << "Received string " << i+1 << ": " << str_to_print << endl;
    }

  //5. Client closes its write end of socket
  status = shutdown(sockfd, SHUT_WR);
  if (status == -1)
    {
      //1. print error message
      perror("shutdown");
      //2. properly clean up
      close(sockfd);
      //3. terminate program on failure
      exit(EXIT_FAILURE);
    }

  //6. Client receives string packet with string "end"
  string end;
  //get_string_from_packet: call()
  status = get_string_from_packet(sockfd, end);
  //get_string_from_packet: check for errors
  if (status == -1)
    {
      //1. print error message
      cout << "Error: get_string_from_packet()" << endl;
      //2. properly clean up
      close(sockfd);
      //3. terminate program on failure
      exit(EXIT_FAILURE);
    }

  //7. Client closes its socket completely
  //close(): call
  status = close(sockfd);
  //close(): check for errors
  if (status == -1)
    {
      //1. print error message
      perror("close");
      //2. terminate program on failure
      exit(EXIT_FAILURE);
    }
  
  return 0;
}
