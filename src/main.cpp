#include "../include/Server.hpp"
#include "../include/Config.hpp"

int main(int ac, char **av)
{
	std::string file;

	ac == 2 ? file = av[1] : file = DEFAULT_CONFIG_FILE;
	try
	{
		Config config(file);
		const std::vector<ServerConfig> & servers = config.getServers();
		for (std::vector<ServerConfig>::const_iterator it = servers.begin(); 
			it != servers.end(); ++it)
		{
			Server server(*it);
			server.run();
		}
	}
	catch (std::exception &e)
	{
		std::cerr << RED("Error: ") << e.what() << std::endl;
	}
	return 0;
}
