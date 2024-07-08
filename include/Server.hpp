#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <netinet/in.h>
#include <cstring>
#include <iostream>

class Server
{
public:
  Server(const char * ipAddress, int port);
  ~Server();

  // initialize the listener (server)
  int init();

  // run the listener (server)
  int run();
protected:

private:
  Server();
  const char* m_ipAddress; //Ip address server will run on
  int m_port; // Port # for the web service
  int m_socket; // Internal FD for the listening socket

};
