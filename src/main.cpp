# include "../include/Server.hpp"

int	main(void)
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
