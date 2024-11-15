//
// Created by vinicius on 11/3/24.
//

#ifndef NETWORKSETUP_HPP
#define NETWORKSETUP_HPP

#include <string>
#include <vector>

class NetworkSetup
{
private:
    std::vector<int> server_sockets;

public:
    NetworkSetup();
    ~NetworkSetup();

    bool setupServer(const std::string& host, int port);
    static int acceptConnection(int server_fd);
    const std::vector<int>& getServerSockets() const;

private:
    // Prevent copying
    NetworkSetup(const NetworkSetup& other);
    NetworkSetup& operator=(const NetworkSetup& other);
};

#endif // NETWORKSETUP_HPP