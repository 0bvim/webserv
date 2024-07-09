# include "../include/Server.hpp"

int	main(void)
{
	try {
		Server server("127.0.0.1", 8080);
    server.run();
	}	catch (std::exception &e)	{
		std::cerr << RED("Error: ") << e.what() << std::endl;
	}
	return 0;
}
