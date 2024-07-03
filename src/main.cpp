# include "Server.hpp"
#include <cstring>

int	main(void)
{
	try {
		Server serv;
	}

	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
