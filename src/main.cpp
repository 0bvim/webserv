#include "../include/Server.hpp"
#include "../include/Config.hpp"

void run(int epoll_fd, epoll_event *event)
{
	while (true)
	{
		int event_count = epoll_wait(epoll_fd, event, MAX_EVENTS, -1);
		if (event_count == -1)
			throw std::runtime_error("Failed to wait on epoll");
		// for (int i = 0; i < event_count; i++)
		// {
		// 	if (_events[i].data.fd == _server_fd)
		// 	{
		// 		if (!this->_acceptClient())
		// 			break;
		// 	}
		// 	else
		// 		this->_handleConnection(_events[i].data.fd);
		// }
	}
}

int main(int ac, char **av)
{
	std::string file;
	int epoll_fd;
	OUTNL("pid " << getpid());

	ac == 2 ? file = av[1] : file = DEFAULT_CONFIG_FILE;
	try
	{
		Config config(file);
		const std::vector<ServerConfig> & servers = config.getServers();
		epoll_fd = epoll_create(MAX_CONNECTIONS);
		epoll_event *event;
		event = new epoll_event[MAX_CONNECTIONS];
		if (epoll_fd == -1)
			throw std::runtime_error("Failed to create epoll file descriptor");
		for (std::vector<ServerConfig>::const_iterator it = servers.begin(); 
			it != servers.end(); ++it)
		{
			Server server(*it, epoll_fd, event);
			OUTNL(server._getServerFd());
			//server.run();
			std::cout << "Server started on port" << std::endl;
		}
		run(epoll_fd, event);
	}
	catch (std::exception &e)
	{
		std::cerr << RED("Error: ") << e.what() << std::endl;
	}
	return 0;
}
