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

	Config config(file);
	config.printServers();
	// try
	// {
	// 	Server server("127.0.0.1", PORT);
	// 	server.run();
	// }
	// catch (std::exception &e)
	// {
	// 	std::cerr << RED("Error: ") << e.what() << std::endl;
	// }

	return 0;
}
