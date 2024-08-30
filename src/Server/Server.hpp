#pragma once

#include <string>
#include <vector>
#include <map>
#include <sys/epoll.h>

class Configuration;

class Server
{
public:
	Server(const Configuration &config);
	void setup();
	void run();

private:
	void createSockets();
	void bindSockets();
	void setupEventLoop();
	void handleIncomingConnections(int server_fd);
	void handleRequest(int client_fd);
	void sendResponse(int client_fd, const std::string &response);
	void serveStaticContent(int client_fd, const std::string &file_path);
	void throwError(int client_fd, int error_code, const std::string &message);
	std::string readFile(const std::string &file_path);

	std::vector<int> server_fds; // Vector of server sockets
	int epoll_fd;
	std::vector<int> ports; // List of ports
	std::string host;
	std::map<std::string, std::string> routes;
	std::vector<struct epoll_event> events;
};
