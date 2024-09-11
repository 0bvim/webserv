#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>

// Configuration Class
class Configuration {
public:
    Configuration();
    bool loadFromFile(const std::string& filename);

    std::vector<int> getPorts() const;
    std::string getHost() const;
    std::map<std::string, std::string> getRoutes() const;

private:
    std::vector<int> ports;
    std::string host;
    std::map<std::string, std::string> routes;

    void parseLine(const std::string& line);
    void addRoute(const std::string& path, const std::string& file);
};

Configuration::Configuration() : host("127.0.0.1") {
    // Default values (host can be modified)
}

bool Configuration::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open configuration file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        parseLine(line);
    }

    file.close();
    return true;
}

void Configuration::parseLine(const std::string& line) {
    std::istringstream iss(line);
    std::string key, value;

    if (!(iss >> key >> value)) {
        return; // Skip empty or malformed lines
    }

    if (key == "port") {
        ports.push_back(std::stoi(value));
    } else if (key == "host") {
        host = value;
    } else if (key == "route") {
        std::string path, file;
        iss >> path >> file;
        addRoute(path, file);
    }
}

void Configuration::addRoute(const std::string& path, const std::string& file) {
    routes[path] = file;
}

std::vector<int> Configuration::getPorts() const {
    return ports;
}

std::string Configuration::getHost() const {
    return host;
}

std::map<std::string, std::string> Configuration::getRoutes() const {
    return routes;
}

// Server Class
class Server {
public:
    Server(const Configuration& config);
    void setup();
    void run();

private:
    void createSockets();
    void bindSockets();
    void setupEventLoop();
    void handleIncomingConnections(int server_fd);
    void handleRequest(int client_fd);
    void sendResponse(int client_fd, const std::string& response);
    void serveStaticContent(int client_fd, const std::string& file_path);
    void throwError(int client_fd, int error_code, const std::string& message);
    std::string readFile(const std::string& file_path);
    
    std::vector<int> server_fds; // Vector of server sockets
    int epoll_fd;
    std::vector<int> ports; // List of ports
    std::string host;
    std::map<std::string, std::string> routes;
    std::vector<struct epoll_event> events;
};

Server::Server(const Configuration& config) : events(10) {
    ports = config.getPorts(); // Retrieve multiple ports from configuration
    host = config.getHost();
    routes = config.getRoutes();
}

void Server::setup() {
    createSockets();
    bindSockets();
    setupEventLoop();
}

void Server::createSockets() {
    for (int port : ports) {
        int server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (server_fd < 0) {
            throw std::runtime_error("Socket creation failed for port " + std::to_string(port));
        }
        server_fds.push_back(server_fd);
    }
}

void Server::bindSockets() {
    for (size_t i = 0; i < server_fds.size(); ++i) {
        int server_fd = server_fds[i];
        int port = ports[i];

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            throw std::runtime_error("Bind failed for port " + std::to_string(port));
        }

        if (listen(server_fd, 10) < 0) {
            throw std::runtime_error("Listen failed for port " + std::to_string(port));
        }

        std::cout << "Server listening on " << host << ":" << port << std::endl;
    }
}

void Server::setupEventLoop() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        throw std::runtime_error("Epoll creation failed");
    }

    for (int server_fd : server_fds) {
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = server_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) < 0) {
            throw std::runtime_error("Epoll control failed for server socket");
        }
    }
}

void Server::run() {
    while (true) {
        int event_count = epoll_wait(epoll_fd, events.data(), events.size(), -1);
        if (event_count < 0) {
            throw std::runtime_error("Epoll wait failed");
        }

        for (int i = 0; i < event_count; ++i) {
            int event_fd = events[i].data.fd;

            bool is_server_fd = false;
            for (int server_fd : server_fds) {
                if (event_fd == server_fd) {
                    is_server_fd = true;
                    break;
                }
            }

            if (is_server_fd) {
                handleIncomingConnections(event_fd);
            } else {
                handleRequest(event_fd);
            }
        }
    }
}

void Server::handleIncomingConnections(int server_fd) {
    sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);

    if (client_fd < 0) {
        std::cerr << "Failed to accept client connection" << std::endl;
        return;
    }

    fcntl(client_fd, F_SETFL, O_NONBLOCK);

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = client_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0) {
        close(client_fd);
        throw std::runtime_error("Failed to add client to epoll");
    }

    std::cout << "New client connected on port: " << server_fd << std::endl;
}

void Server::handleRequest(int client_fd) {
    char buffer[1024];
    int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0) {
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
        close(client_fd);
        std::cout << "Client disconnected: " << client_fd << std::endl;
        return;
    }

    buffer[bytes_read] = '\0';
    std::string request(buffer);
    std::cout << "Request received:\n" << request << std::endl;

    // Parse HTTP request and determine action
    if (request.find("GET") == 0) {
        std::string request_path = request.substr(4, request.find(" ", 4) - 4);
		std::cout << "this is the request path " << request_path << std::endl;

        if (routes.find(request_path) != routes.end()) {
            serveStaticContent(client_fd, routes[request_path]);
        } else {
            throwError(client_fd, 404, "File Not Found");
        }
    } else {
        throwError(client_fd, 501, "Not Implemented");
    }
}

void Server::serveStaticContent(int client_fd, const std::string& file_path) {
    std::string content = readFile(file_path);
	std::cout << "this is the path " << file_path << std::endl;

    if (content.empty()) {
        throwError(client_fd, 404, "File Not Found");
        return;
    }

    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
    response += "Content-Type: text/html\r\n";
    response += "\r\n";
    response += content;

    sendResponse(client_fd, response);
}

void Server::sendResponse(int client_fd, const std::string& response) {
    send(client_fd, response.c_str(), response.size(), 0);
}

void Server::throwError(int client_fd, int error_code, const std::string& message) {
    std::string response = "HTTP/1.1 " + std::to_string(error_code) + " " + message + "\r\n";
    response += "Content-Length: " + std::to_string(message.size()) + "\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "\r\n";
    response += message;

    sendResponse(client_fd, response);
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
    close(client_fd);
}

std::string Server::readFile(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::in | std::ios::binary);
    if (!file) {
        return "";
    }

    std::ostringstream content;
    content << file.rdbuf();
    file.close();
    return content.str();
}

// Main function
int main() {
    Configuration config;
    if (!config.loadFromFile("server.conf")) {
        std::cerr << "Failed to load configuration" << std::endl;
        return 1;
    }

    try {
        Server server(config);
        server.setup();
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
