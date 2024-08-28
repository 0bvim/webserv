// #include <iostream>
// #include <fstream>
// #include <map>
// #include <string>
// #include <vector>

// struct Location
// {
// 	std::string path;
// 	std::map<std::string, std::string> directives;
// };

// struct Server
// {
// 	std::string server_name;
// 	std::string listen;
// 	std::map<std::string, std::string> directives;
// 	std::vector<Location> locations;
// };

// struct NginxConfig
// {
// 	std::vector<Server> servers;
// };

// class NginxParser
// {
// public:
// 	NginxConfig parseConfig(const std::string &filename)
// 	{
// 		std::ifstream file(filename.c_str());
// 		NginxConfig config;
// 		std::string line;
// 		Server currentServer;
// 		Location currentLocation;
// 		bool inServerBlock = false;
// 		bool inLocationBlock = false;

// 		while (std::getline(file, line))
// 		{
// 			line = trim(line);

// 			if (line.empty() || line[0] == '#')
// 				continue; // Skip comments
// 			if (line == "server {")
// 			{
// 				inServerBlock = true;
// 				currentServer = Server();
// 			}
// 			else if (line == "}")
// 			{
// 				if (inLocationBlock)
// 				{
// 					currentServer.locations.push_back(currentLocation);
// 					inLocationBlock = false;
// 				}
// 				else if (inServerBlock)
// 				{
// 					config.servers.push_back(currentServer);
// 					inServerBlock = false;
// 				}
// 			}
// 			else if (line.find("location") == 0)
// 			{
// 				inLocationBlock = true;
// 				currentLocation = Location();
// 				currentLocation.path = line.substr(9, line.find("{") - 9);
// 				currentLocation.path = trim(currentLocation.path);
// 			}
// 			else if (inLocationBlock)
// 			{
// 				size_t pos = line.find(' ');
// 				std::string directive = line.substr(0, pos);
// 				std::string value = trim(line.substr(pos + 1));
// 				currentLocation.directives[directive] = value;
// 			}
// 			else if (inServerBlock)
// 			{
// 				size_t pos = line.find(' ');
// 				std::string directive = line.substr(0, pos);
// 				std::string value = trim(line.substr(pos + 1));
// 				if (directive == "server_name")
// 					currentServer.server_name = value;
// 				else if (directive == "listen")
// 					currentServer.listen = value;
// 				else
// 					currentServer.directives[directive] = value;
// 			}
// 		}
// 		return config;
// 	}

// private:
// 	std::string trim(const std::string &s)
// 	{
// 		size_t start = s.find_first_not_of(" \t");
// 		size_t end = s.find_last_not_of(" \t");
// 		return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
// 	}
// };

// int main()
// {
// 	NginxParser parser;
// 	NginxConfig config = parser.parseConfig("empty.conf");

// 	for (size_t i = 0; i < config.servers.size(); ++i)
// 	{
// 		const Server &server = config.servers[i];
// 		std::cout << "Server " << i + 1 << ":\n";
// 		std::cout << "  server_name: " << server.server_name << "\n";
// 		std::cout << "  listen: " << server.listen << "\n";
// 		for (std::map<std::string, std::string>::const_iterator it = server.directives.begin(); it != server.directives.end(); ++it)
// 			std::cout << "  " << it->first << ": " << it->second << "\n";
// 		for (size_t j = 0; j < server.locations.size(); ++j)
// 		{
// 			const Location &location = server.locations[j];
// 			std::cout << "  Location " << j + 1 << ": " << location.path << "\n";
// 			for (std::map<std::string, std::string>::const_iterator it = location.directives.begin(); it != location.directives.end(); ++it)
// 				std::cout << "    " << it->first << ": " << it->second << "\n";
// 		}
// 	}
// 	return 0;
// }
To make this code compatible with C++98 while maintaining the same behavior, you'll need to replace modern C++ features with alternatives that are available in C++98. Here's the adjusted code:

```cpp
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
#include <errno.h>

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
    std::ifstream file(filename.c_str());
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
        ports.push_back(atoi(value.c_str()));
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
    for (size_t i = 0; i < ports.size(); ++i) {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            throw std::runtime_error("Socket creation failed for port " + std::to_string(ports[i]));
        }

        // Set non-blocking mode
        int flags = fcntl(server_fd, F_GETFL, 0);
        if (flags < 0) {
            close(server_fd);
            throw std::runtime_error("Failed to get socket flags for port " + std::to_string(ports[i]));
        }
        if (fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
            close(server_fd);
            throw std::runtime_error("Failed to set socket flags for port " + std::to_string(ports[i]));
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
    epoll_fd = epoll_create(1);
    if (epoll_fd < 0) {
        throw std::runtime_error("Epoll creation failed");
    }

    for (size_t i = 0; i < server_fds.size(); ++i) {
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = server_fds[i];
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fds[i], &event) < 0) {
            throw std::runtime_error("Epoll control failed for server socket");
        }
    }
}

void Server::run() {
    while (true) {
        int event_count = epoll_wait(epoll_fd, &events[0], events.size(), -1);
        if (event_count < 0) {
            throw std::runtime_error("Epoll wait failed");
        }

        for (int i = 0; i < event_count; ++i) {
            int event_fd = events[i].data.fd;

            bool is_server_fd = false;
            for (size_t j = 0; j < server_fds.size(); ++j) {
                if (event_fd == server_fds[j]) {
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
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
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

    std::ostringstream oss;
   

 oss << "HTTP/1.1 200 OK\r\n";
    oss << "Content-Length: " << content.size() << "\r\n";
    oss << "Content-Type: text/html\r\n";
    oss << "Connection: close\r\n";
    oss << "\r\n";
    oss << content;

    sendResponse(client_fd, oss.str());
}

void Server::sendResponse(int client_fd, const std::string& response) {
    write(client_fd, response.c_str(), response.size());
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
    close(client_fd);
}

void Server::throwError(int client_fd, int error_code, const std::string& message) {
    std::ostringstream oss;
    oss << "HTTP/1.1 " << error_code << " " << message << "\r\n";
    oss << "Content-Length: " << message.size() << "\r\n";
    oss << "Content-Type: text/html\r\n";
    oss << "Connection: close\r\n";
    oss << "\r\n";
    oss << message;

    sendResponse(client_fd, oss.str());
}

std::string Server::readFile(const std::string& file_path) {
    std::ifstream file(file_path.c_str());
    if (!file.is_open()) {
        return "";
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    file.close();
    return oss.str();
}

int main() {
    Configuration config;
    if (!config.loadFromFile("server.conf")) {
        std::cerr << "Failed to load configuration" << std::endl;
        return 1;
    }

    Server server(config);
    server.setup();
    server.run();

    return 0;
}
