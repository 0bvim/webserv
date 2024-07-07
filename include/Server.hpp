#pragma once

class Server
{
public:

protected:

private:
  const char* m_ipAddress; //Ip address server will run on
  int m_port; // Port # for the web service
  int m_socket; // Internal FD for the listening socket

};
