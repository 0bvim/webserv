/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:35 by bmoretti          #+#    #+#             */
/*   Updated: 2024/08/03 14:49:55 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(Config &config) : _config(config)
{
	this->_status = HttpStatus::ZERO;
	this->_address = config.getServerAddress();
	this->_port = config.getServerPort();
	this->_server_fd = -1;
	this->_epoll_fd = -1;
	this->_initServer();
	OUTNL(MAGENTA("Read to connect in: " << _address + ":" << _port));
}

Server::~Server()
{
	if (_server_fd != -1)
		close(_server_fd);
	if (_epoll_fd != -1)
		close(_epoll_fd);
}

bool Server::_acceptClient()
{
	while (true)
	{
		sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_fd = accept(_server_fd, (sockaddr *)&client_addr, &client_addr_len);
		if (client_fd == -1)
			if (!this->_handleAcceptError(errno))
				return false;
		this->_setNonBlocking(client_fd);
		epoll_event event;
		memset(&event, 0, sizeof(epoll_event));
		event.events = EPOLLIN | EPOLLET;
		event.data.fd = client_fd;
		if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
		{
			std::cerr << "Failed to add client socket to epoll" << std::endl;
			close(client_fd);
		}
	}
}

void Server::run()
{
	while (true)
	{
		int event_count = epoll_wait(_epoll_fd, _events, MAX_EVENTS, -1);
		if (event_count == -1)
			throw std::runtime_error("Failed to wait on epoll");
		for (int i = 0; i < event_count; i++)
		{
			if (_events[i].data.fd == _server_fd)
			{
				if (!this->_acceptClient())
					break;
			}
			else
				this->_handleConnection(_events[i].data.fd);
		}
	}
}

bool Server::_handleAcceptError(int error_code)
{
	if (error_code == EAGAIN || error_code == EWOULDBLOCK)
		return false;
	else
	{
		std::cerr << "Failed to accept client connection" << std::endl;
		return false;
	}
	return true;
}

void Server::_handleConnection(int client_fd)
{
	char buffer[BUFFER_SIZE];
	while (true)
	{
		ssize_t bytes_read = _readFromClient(client_fd, buffer);
		if (bytes_read <= 0)
			break;
		else
		{
			buffer[bytes_read] = '\0';
			_fillBuffer(client_fd, buffer);
			if (this->_status == HttpStatus::BAD_REQUEST)
			{
				this->_printOnClient(client_fd, "Bad Request\n");
				close(client_fd);
			}
			else if (_checkEndMessage(client_fd))
			{
				Request request(this->_buffer_request[client_fd].c_str());
				//request.printRequest();
				Response response(request, this->_config);
				this->_printOnClient(client_fd, response.getResponse());
			}
		}
	}
}

void Server::_setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("Failed to get file descriptor flags");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("Failed to set non-blocking mode");
}
