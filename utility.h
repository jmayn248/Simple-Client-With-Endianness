#ifndef UTILITY_H
#define UTILITY_H

using namespace std;

//
// Function name: create_and_connect_socket
// Description:
// Creates a socket, and connects the socket to
// a specific host and port number
// Parameters:
// s-tring host: host internet address
// -string port: port number
// Return Value: int
// -on success: socket file descriptor
// -on failure: -1
//
int create_and_connect_socket(string host, string port);

//
// Function name: sendall
// Description:
// Sends all of an unsigned long integer
// Parameters
// -int sockfd: socket file descriptor
// -uint32_t msg: unsigned long integer to send
// Return Value: int
// -on success, number of bytes sent
// -on failure, -1
//
int sendall(int sockfd, uint32_t msg);

//
// Function name: recvn
// Description:
// Receive n number of bytes, and assign those bytes into a string, msg
// Parameters
// -int sockfd: socket file descriptor
// -int n: number of bytes to receive
// -string& msg: string to assign bytes to
// Return Value: int
// -on success, number of bytes read
// -on failure, -1
//
int recvn(int sockfd, int n, string& msg);

//
// Function name: get_string_from_packet
// Description:
// 1. Gets length of string packet with recvn()
// 2. Parses length of string packet
// 3. Gets ASCII characters of string packet
// 4. Assigns ASCII characters of string packet to string msg
// Parameters
// -int sockfd: socket file descriptor
// -int &msg: string to assign ASCII cahracters of string packet to
// Return Value: int
// -on success, 0
// -on failure, -1
//
int get_string_from_packet(int sockfd, string &msg);

#endif
