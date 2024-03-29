#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
using namespace std;

// helper function prototypes
#include "client-helpers.h"

int main(int argc, char const *argv[]) {
  // check if user provided hostname and port number
  if (argc < 3) {
    cout << "Error: wrong format\nCorrect format: " << argv[0] << " hostname port" << endl;
    exit(1);
  }
  else if (argc > 3) {
    error("Too many command-line arguments provided.\n");
  }

  int port_no = atoi(argv[2]);
  int n;
  struct sockaddr_in serv_addr; // sockaddr_in - structure containing internet address
  struct hostent *server; // pointer to struct of type hostent - defines host computer on Internet

  // create a socket - socket()
  int client_fd = socket(AF_INET, SOCK_STREAM, 0); // 0 is the protocol parameter
  if (client_fd < 0) {
    error("An error occurred trying to connect to the server. Exiting...\n");
  }
  cout << "Socket created" << endl;

  // connect socket to address of the server - connect()
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    cout << "Error: no such host\n" << endl;
    exit(0);
  }
  cout << "Connecting to server..." << endl;

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(port_no);

  if (connect(client_fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error("Error connecting to the server\n");
  }
  cout << "Connected to server" << endl;

  // send and receive data - read() / write()
  chat(client_fd);

  return 0;
}

// Sources:
//--- https://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
//--- https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
