#include <iostream>
#include <stdexcept>
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
