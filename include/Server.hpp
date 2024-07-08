#pragma once

#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <vector>
#include <netinet/in.h>
#include <cstring>
#include <iostream>

class Server
{
public:
  Server(std::string ipAddress, int port);
  virtual ~Server() {}

  // initialize the listener (server)
  int init();

  // run the listener (server)
  int run();
protected:
/*   handler for client connection */
  virtual void onClientConnected(int position);
/*   handler for client disconnection */

  virtual void onClientDisconnected(int position);

/*   handler for client messages */
  virtual void onMessageReceived(int clientSocket, const char *msg, int length);

/*   send a message to client */
  virtual void sendToClient(int clientSocket, const char *msg, int length);

private:
  Server() {}
  std::string m_ipAddress; //Ip address server will run on
  int m_port; // Port # for the web service
  int m_socket; // Internal FD for the listening socket
  fd_set m_master; // master file descriptor set
};
