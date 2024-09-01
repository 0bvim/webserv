/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 16:09:46 by bmoretti          #+#    #+#             */
/*   Updated: 2024/08/31 17:50:13 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Events.hpp"

Events::Events(const std::vector<ServerConfig> &servers)
{
	this->_epoll_fd = epoll_create(MAX_CONNECTIONS);
	this->_event = new epoll_event[MAX_CONNECTIONS];
	memset(this->_event, 0, sizeof(epoll_event) * MAX_CONNECTIONS);
	this->_event->events = EPOLLIN | EPOLLET;
	if (this->_epoll_fd == -1)
		throw std::runtime_error("Failed to create epoll file descriptor");
	for (std::vector<ServerConfig>::const_iterator it = servers.begin();
		 it != servers.end(); ++it)
	{
		Server *server = new Server(*it, this->_epoll_fd, this->_event);
		this->_servers.push_back(server);
	}
}

Events::~Events()
{
	for (std::vector<Server *>::iterator it = this->_servers.begin();
		 it != this->_servers.end(); ++it)
		delete *it;
	if (this->_epoll_fd != -1)
		close(this->_epoll_fd);
	if (this->_event)
		delete[] this->_event;
}

void Events::run()
{
	while (true)
	{
		int event_count = epoll_wait(this->_epoll_fd, this->_event, MAX_EVENTS, -1);
		if (event_count == -1)
			throw std::runtime_error("Failed to wait on epoll");
		for (int i = 0; i < event_count; i++)
		{
			bool found = false;
			for(int j = 0, size = this->_servers.size(); j < size; j++)
			{
				int server_fd = this->_servers[j]->_getServerFd();
				if (_event[i].data.fd == server_fd && this->_servers[j]->_acceptClient())
				{
					found = true;
					break;
				}
				else if (this->_servers[j]->_isClientConnected(_event[i].data.fd))
				{
					this->_servers[j]->_handleConnection(_event[i].data.fd);
					found = true;
					break;
				}
			}
			if (found)
				break;
		}
	}
}


void Events::_handleEvents()
{


}
