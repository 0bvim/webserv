# include "../include/Server.hpp"

// a main tem que receber argv para conseguirmos verificar o arquivo.
int	main(int ac, char **av)
{
	Config config("arquivo.conf");
	config.printServers();

	// try {
	// 	Server server("127.0.0.1", PORT);
    // server.run();
	// }	catch (std::exception &e)	{
	// 	std::cerr << RED("Error: ") << e.what() << std::endl;
	// }
	// return 0;
}
