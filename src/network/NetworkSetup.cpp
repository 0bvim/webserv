//
// Created by vinicius on 11/3/24.
//

#include "NetworkSetup.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <cerrno>
#include <unistd.h> // for close()

NetworkSetup::NetworkSetup()
{
}

NetworkSetup::~NetworkSetup()
{
    for (std::vector<int>::iterator it = server_sockets.begin(); it != server_sockets.end(); ++it)
        if (*it >= 0)
            close(*it);
}

bool NetworkSetup::setupServer(const std::string& host, int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (server_fd < 0)
        return false;

    // Allow reuse of address
    const int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        close(server_fd);
        return false;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY; // Allow connections on any interface

    // If specific host is provided, use it
    if (host != "0.0.0.0" && !host.empty())
    {
        if (inet_pton(AF_INET, host.c_str(), &address.sin_addr) <= 0)
        {
            close(server_fd);
            return false;
        }
    }

    // Bind socket to address and port
    if (bind(server_fd, reinterpret_cast<struct sockaddr*>(&address),
             sizeof(address)) < 0)
    {
        close(server_fd);
        return false;
    }

    // Start listening
    if (listen(server_fd, SOMAXCONN) < 0)
    {
        close(server_fd);
        return false;
    }

    server_sockets.push_back(server_fd);
    return true;
}

int NetworkSetup::acceptConnection(int server_fd)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int client_fd = accept(server_fd,
                           reinterpret_cast<struct sockaddr*>(&client_addr),
                           &addr_len);

    // TODO: verify correct return if client_fd is equal to `-1`
    if (client_fd < 0)
        return -1;

    // Make client socket non-blocking
    int flags = 1;
    if (ioctl(client_fd, FIONBIO, &flags) < 0)
    {
        close(client_fd);
        return -1;
    }

    return client_fd;
}

const std::vector<int>& NetworkSetup::getServerSockets() const
{
    return server_sockets;
}