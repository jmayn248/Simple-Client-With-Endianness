//Citations
//1. getaddrinfo() example code
//Source: Linux Programmer's Manual (man 3 getaddrinfo)
//Retrieval Date: November 7, 2024
//Note: modified for use in create_and_connect_socket()
//2. sendall()
//Author: Dr. Dylan Schwesinger
//Source: Dr. Schwesinger's public directory on acad.kutztown.edu
//Retrieval Date: November 7, 2024
//Note: based on Citation 3
//3. 7.4 Handling Partial sends()
//Author: Brian "Beej Jorgensen" Hall
//Copyright: July 17, 2024
//Source: https://beej.us/guide/bgnet/html/split/slightly-advanced-techniques.html#sendall
//Retrieval Date: November 7, 2024
//4. recvall()
//Author: Dr. Dylan Schwesinger
//Source: Dr. Schwesinger's public directory on acad.kutztown.edu
//Retrieval Date: November 7, 2024
//Note: modified recvall() to receive n number of bytes -> recvn()
//5. sizeof()
//Source: https://en.cppreference.com/w/cpp/language/sizeof
//Retrieval Date: November 8, 2024
//6. string()
//Source: https://cplusplus.com/reference/string/string/string/
//Retreval Date: November 8, 2024
//7. "How do I concatenate 2 bytes?"
//Source: https://stackoverflow.com/questions/1935449/how-do-i-concatenate-2-bytes
//Retrieval Date: November 8, 2024
//8. Fundamental Types
//Source: https://en.cppreference.com/w/cpp/language/types
//Retrieval Date: November 8, 2024
//used to determine an alternative to byte data type from code in Citation 7

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

#include "utility.h"

using namespace std;

int create_and_connect_socket(string host, string port)
{
  int sockfd = -1; //socket file descriptor
  struct addrinfo hints; //criteria for selecting addrinfo structs
  struct addrinfo *res; //points to set of addrinfo structs returned by getaddrinfo()
  int status; //stores return values of lib funcs for error checking

  //0. LOAD UP SOCKET STRUCTS
  memset(&hints, 0, sizeof hints); //set all fields to 0
  hints.ai_family = AF_INET; //IPv4
  hints.ai_socktype = SOCK_STREAM; //TCP

  //getaddrinfo(): call
  status = getaddrinfo(host.c_str(), //convert host to C string so can pass as arg
                       port.c_str(),
                       &hints, &res);
  //make res point to 1 or more addrinfo structs
  //that meet criteria in first 3 args

  //getaddrinfo(): check for errors
  //Citation 1
  //-------------------------------------------------------------
  if (status != 0) {
    //1. print error message
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    //2. properly clean up
    //NOT REQUIRED
    //3. return -1 on failure
    return -1;
  }
  //-------------------------------------------------------------

  //1. CREATE SOCKET
  //socket(): call
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  //create a socket file descriptor that
  //meets fields of first addrinfo struct
  //pointed to by res

  //socket(): check for errors
  if (sockfd == -1)
    {
      //1. print error message
      perror("socket");
      //2. properly clean up
      freeaddrinfo(res);
      //3. return -1 on failure
      return -1;
    }

  //2. CONNECT SOCKET
  //connect(): call
  status = connect(sockfd, res->ai_addr, res->ai_addrlen);
  //connect socket represented by sockfd
  //to address of first addrinfo struct
  //pointed to by res

  //connect(): check for errors
  if (status == -1)
    {
      //1. print error message
      perror("connect");
      //2. properly clean up
      freeaddrinfo(res);
      close(sockfd);
      //3. return -1 on failure
      return -1;
    }

  //SUCCESS
  //1. properly clean up before return
  freeaddrinfo(res);
  //2. return
  return sockfd; //socket file descriptor
}

//Citation 2
//---------------------------------------------------------------
int sendall(int sockfd, uint32_t msg) {
  const uint32_t* buf = &msg;
  int len = sizeof(msg);
  int total = 0;        // how many bytes we've sent
  int bytesleft = len;  // how many we have left to send
  int n;

  while(total < len) {
    //send(): call
    n = send(sockfd, buf+total, bytesleft, 0);
    //send(): check for errors
    if (n == -1) { break; }
    total += n;
    bytesleft -= n;
  }

  // return -1 on failure, number of bytes sent on success
  return n == -1 ? -1 : total;
}

//---------------------------------------------------------------

//Citation 4
//---------------------------------------------------------------
int recvn(int sockfd, int n, string& msg)
{
  const size_t chunk_size = 65536;
  char buf[chunk_size];
  stringstream result;
  int len;
  int bytes_read = 0;

  while (bytes_read < n)
    {
      //recv(): call
      len = recv(sockfd, buf, n, 0);

      //recv():
      if (len == 0) { break; }

      //recv(): check for errors
      if (len == -1) { break; }

      //write results to buffer
      buf[len+1] = '\0';
      result << string(buf, len);

      bytes_read += len;

    }

  //recv(): check for errors
  if (len == -1) {
    //1. print error message
    stringstream s;
    s << "recv: " << strerror(errno) << endl;
    //2. return error
    return -1;
  }

  //SUCCESS
  //return
  msg = result.str();
  return bytes_read;
}

//---------------------------------------------------------------



int get_string_from_packet(int sockfd, string &msg)
{
  int status;
  string temp;

  //recvn(): call
  status = recvn(sockfd, 2, temp);

  //recvn(): check for errors
  if (status == -1)
    {
      //1. print error message
      cout << "Error: recvn()" << endl;
      //2. return error
      return -1;
    }

  uint16_t str_length;

  //Citation 7
  //-------------------------------------------------------------
  unsigned char b1 = temp[0];
  unsigned char b2 = temp[1];

  str_length = b1 << 8 | b2;

  //-------------------------------------------------------------

  //recvn(): call
  status = recvn(sockfd, str_length, temp);

  //recvn(): check for errors
  if (status == -1)
    {
      //1. print error message
      cout << "Error: recvn()" << endl;
      //2. return error
      return -1;
    }

  //SUCCESS
  //return
  msg = temp;
  return 0;
}
