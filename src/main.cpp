#include "../include/Server.hpp"
#include "../include/Config.hpp"
#include <string>

// a main tem que receber argv para conseguirmos verificar o arquivo.
int main(int ac, char **av)
{
	std::string file;

	if (ac == 2)
		file = av[1];
	else
		file = DEFAULT_CONFIG_FILE;

	try
	{
		Config config(file);
		// Server server(config.getServerAddress(), config.getServerPort());
		// OUTNL("address: " << config.getServerAddress());
		// OUTNL("PORT: " << config.getServerPort());
		Server server(config.getServerAddress(), config.getServerPort());
		server.run();
	}
	catch (std::exception &e)
	{
		std::cerr << RED("Error: ") << e.what() << std::endl;
	}

	return 0;
}
