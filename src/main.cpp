#include "../include/Server.hpp"
#include "../include/Config.hpp"
#include "../include/Events.hpp"

int main(int ac, char **av)
{
	std::string file;

	ac == 2 ? file = av[1] : file = DEFAULT_CONFIG_FILE;
	try
	{
		Config config(file);
		Events events(config.getServers());
		events.run();
	}
	catch (std::exception &e)
	{
		std::cerr << RED("Error: ") << e.what() << std::endl;
	}
	return 0;
}
