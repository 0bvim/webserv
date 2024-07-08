#include "../include/Server.hpp"

Server::Server(const char * ipAddress, int port) : m_ipAddress(ipAddress), m_port(port) {}

int Server::init()
{
  // Create a socket
  m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (m_socket == -1)
  {
    std::cerr << "Failed to create socket" << std::endl;
    return errno;
  }

  // Bind the ip address and port to a socket
  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(m_port);
  inet_pton(AF_INET, m_ipAddress, &hint.sin_addr);

  if (bind(m_socket, (sockaddr*)&hint, sizeof(hint)) == -1)
  {
    std::cerr << "Failed to bind to IP/Port" << std::endl;
    return errno;
  }

  // Tell the socket to listen
  if (listen(m_socket, SOMAXCONN) == -1)
  {
    std::cerr << "Failed to listen on socket" << std::endl;
    return errno;
  }

  // Create the master file descriptor set and zero it
  FD_ZERO(&m_master);

  // Add our first socket that we're interested in interacting with; the listening socket!
  // It's important that this socket is added for our server or else we won't 'hear' incoming
  // connections 
  FD_SET(m_socket, &m_master);

  return 0;
}

int Server::run()
{
  // This will be changed by the \quit command (see below, bonus not in video!)
  bool running = true;

  while (running)
  {
    // Make a copy of the master file descriptor set, this is SUPER important because
    // the call to select() is _DESTRUCTIVE_. The copy only contains the sockets that
    // are accepting inbound connection requests OR messages. 

    fd_set copy = m_master;

    // See who's talking to us
    int socketCount = select(m_socket + 1, &copy, nullptr, nullptr, nullptr);
    if (socketCount < 0)
    {
      std::cerr << "select() failed" << std::endl;
      return errno;
    }

    // Loop through all the current connections / potential connect
    for (int i = 0; i <= m_socket; i++)
    {
      // Check if this socket is ready to read
      if (FD_ISSET(i, &copy))
      {
        // Is it an inbound communication?
        if (i == m_socket)
        {
          // Accept a new connection
          int client = accept(m_socket, nullptr, nullptr);
          if (client < 0)
          {
            std::cerr << "accept() failed" << std::endl;
            return errno;
          }

          // Add the new connection to the list of connected clients
          FD_SET(client, &m_master);
          if (client > m_socket)
          {
            m_socket = client;
          }

          onClientConnected(client);
        }
        else // It's an inbound message
        {
          char buf[4096];
          memset(buf, 0, 4096);

          // Receive message
          int bytesIn = recv(i, buf, 4096, 0);
          if (bytesIn <= 0)
          {
            // Drop the client
            onClientDisconnected(i);
            close(i);
            FD_CLR(i, &m_master);
          }
          else
        {
            onMessageReceived(i, buf, bytesIn);
          }
        }
      }
    }
  }
  // Remove the listening socket from the master file descriptor set and close it
  // to prevent anyone else trying to connect.
  FD_CLR(m_socket, &m_master);
  close(m_socket);

  for (int i = 0; i <= m_socket; i++)
  {
    if (FD_ISSET(i, &m_master))
    {
      // Remove it from the master file list and close the socket
      FD_CLR(i, &m_master);
      close(i);
    }
  }

  return 0;
}

void Server::sendToClient(int clientSocket, const char *msg, int length)
{
  send(clientSocket, msg, length, 0);


}

// #include <sys/epoll.h>
// this run are with epoll

// int TcpListener::run()
// {
//     // Create epoll instance
//     int epoll_fd = epoll_create1(0);
//     if (epoll_fd == -1) {
//         std::cerr << "Failed to create epoll instance" << std::endl;
//         return errno;
//     }

//     // Add the listening socket to the epoll instance
//     struct epoll_event event;
//     event.events = EPOLLIN;
//     event.data.fd = m_socket;
//     if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_socket, &event) == -1) {
//         std::cerr << "Failed to add listening socket to epoll instance" << std::endl;
//         close(epoll_fd);
//         return errno;
//     }

//     // Buffer to store events
//     constexpr int MAX_EVENTS = 10;
//     std::vector<struct epoll_event> events(MAX_EVENTS);

//     // Event loop
//     while (true) {
//         // Wait for events
//         int num_events = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
//         if (num_events == -1) {
//             std::cerr << "epoll_wait failed" << std::endl;
//             close(epoll_fd);
//             return errno;
//         }

//         // Handle events
//         for (int i = 0; i < num_events; ++i) {
//             if (events[i].data.fd == m_socket) {
//                 // New connection
//                 struct sockaddr_in client_addr;
//                 socklen_t client_len = sizeof(client_addr);
//                 int client_fd = accept(m_socket, (struct sockaddr *)&client_addr, &client_len);
//                 if (client_fd == -1) {
//                     std::cerr << "Failed to accept new connection" << std::endl;
//                     continue;
//                 }

//                 // Add new client socket to epoll instance
//                 event.events = EPOLLIN | EPOLLET; // Edge-triggered mode
//                 event.data.fd = client_fd;
//                 if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
//                     std::cerr << "Failed to add client socket to epoll instance" << std::endl;
//                     close(client_fd);
//                     continue;
//                 }

//                 onClientConnected(client_fd);
//             } else {
//                 // Handle client socket
//                 if (events[i].events & EPOLLIN) {
//                     // Incoming data
//                     char buf[4096];
//                     memset(buf, 0, sizeof(buf));
//                     int bytes_read = recv(events[i].data.fd, buf, sizeof(buf), 0);
//                     if (bytes_read == -1) {
//                         std::cerr << "Error reading from client socket" << std::endl;
//                         close(events[i].data.fd);
//                         continue;
//                     } else if (bytes_read == 0) {
//                         // Connection closed
//                         onClientDisconnected(events[i].data.fd);
//                         close(events[i].data.fd);
//                         epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
//                         continue;
//                     }

//                     // Handle received data
//                     onMessageReceived(events[i].data.fd, buf, bytes_read);
//                 }
//             }
//         }
//     }

//     close(epoll_fd);
//     return 0;
// }
// #include <sys/epoll.h>
// #include <unistd.h>
// #include <cstring>
// #include <iostream>
// #include <vector>

// int TcpListener::run()
// {
//     // Create epoll instance
//     int epoll_fd = epoll_create1(0);
//     if (epoll_fd == -1) {
//         std::cerr << "Failed to create epoll instance" << std::endl;
//         return errno;
//     }

//     // Add the listening socket to the epoll instance
//     struct epoll_event event;
//     event.events = EPOLLIN;
//     event.data.fd = m_socket;
//     if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_socket, &event) == -1) {
//         std::cerr << "Failed to add listening socket to epoll instance" << std::endl;
//         close(epoll_fd);
//         return errno;
//     }

//     // Buffer to store events
//     constexpr int MAX_EVENTS = 10;
//     std::vector<struct epoll_event> events(MAX_EVENTS);

//     // Event loop
//     while (true) {
//         // Wait for events
//         int num_events = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
//         if (num_events == -1) {
//             std::cerr << "epoll_wait failed" << std::endl;
//             close(epoll_fd);
//             return errno;
//         }

//         // Handle events
//         for (int i = 0; i < num_events; ++i) {
//             if (events[i].data.fd == m_socket) {
//                 // New connection
//                 struct sockaddr_in client_addr;
//                 socklen_t client_len = sizeof(client_addr);
//                 int client_fd = accept(m_socket, (struct sockaddr *)&client_addr, &client_len);
//                 if (client_fd == -1) {
//                     std::cerr << "Failed to accept new connection" << std::endl;
//                     continue;
//                 }

//                 // Add new client socket to epoll instance
//                 event.events = EPOLLIN | EPOLLET; // Edge-triggered mode
//                 event.data.fd = client_fd;
//                 if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
//                     std::cerr << "Failed to add client socket to epoll instance" << std::endl;
//                     close(client_fd);
//                     continue;
//                 }

//                 onClientConnected(client_fd);
//             } else {
//                 // Handle client socket
//                 if (events[i].events & EPOLLIN) {
//                     // Incoming data
//                     char buf[4096];
//                     memset(buf, 0, sizeof(buf));
//                     int bytes_read = recv(events[i].data.fd, buf, sizeof(buf), 0);
//                     if (bytes_read == -1) {
//                         std::cerr << "Error reading from client socket" << std::endl;
//                         close(events[i].data.fd);
//                         continue;
//                     } else if (bytes_read == 0) {
//                         // Connection closed
//                         onClientDisconnected(events[i].data.fd);
//                         close(events[i].data.fd);
//                         epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
//                         continue;
//                     }

//                     // Handle received data
//                     onMessageReceived(events[i].data.fd, buf, bytes_read);
//                 }
//             }
//         }
//     }

//     close(epoll_fd);
//     return 0;
// }

