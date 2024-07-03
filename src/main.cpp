# include "Server.hpp"

bool g_sigint = false;

void	handle_signal(int c)
{
	(void)c;
	g_sigint = true;
	OUTNL(RED("\r[HALT]: STOPPING THE SERVER... \n"));
}

int	main(void)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, handle_signal);

	try {
		Server serv;
	}

	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
