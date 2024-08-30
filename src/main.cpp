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
#include "Server/Server.hpp"
#include "Configuration/Configuration.hpp"



// Main function
int main()
{
	Configuration config;
	if (!config.loadFromFile("server.conf"))
	{
		std::cerr << "Failed to load configuration" << std::endl;
		return 1;
	}

	try
	{
		Server server(config);
		server.setup();
		server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Server error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
