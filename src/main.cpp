# include "Server.hpp"
# include <csignal>

int	main(void)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, Server::_handleSignal);

	try {
		Server serv;
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
