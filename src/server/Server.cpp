//
// Created by vinicius on 11/3/24.
//

#include "Server.hpp"
#include <iostream>

Server_class::Server_class() : _is_running(false), port(0), client_max_size(0) {}

Server_class::~Server_class() {
    if (_is_running) {
        stop();
    }
}

bool Server_class::start() {
    if (_is_running) {
        return false;
    }

    // Validate server configuration
    if (host.empty() || port <= 0 || port > 65535) {
        std::cerr << "Invalid server configuration" << std::endl;
        return false;
    }

    // Initialize network setup
    if (!_network.setupServer(host, port)) {
        std::cerr << "Failed to setup server on " << host << ":" << port << std::endl;
        return false;
    }

    _is_running = true;
    return true;
}

void Server_class::stop() {
    _is_running = false;
}

bool Server_class::isRunning() const {
    return _is_running;
}

int Server_class::acceptClient() {
    if (!_is_running) {
        return -1;
    }

    const std::vector<int>& sockets = _network.getServerSockets();
    if (!sockets.size()) {
        return -1;
    }

    // For now, we'll just use the first socket
    // You might want to implement a more sophisticated approach for multiple sockets
    return _network.acceptConnection(sockets[0]);
}

const std::vector<int>& Server_class::getServerSockets() const {
    return _network.getServerSockets();
}