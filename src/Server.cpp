/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:35 by bmoretti          #+#    #+#             */
/*   Updated: 2024/08/17 16:21:49 bbbnivicius      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(const ServerConfig &config, int epoll_fd, epoll_event *event) : _config(config)
{
	this->_status = HttpStatus::ZERO;
	this->_address = config.address;
	this->_port = config.port;
	this->_server_fd = -1;
	this->_epoll_fd = epoll_fd;
	this->_event = event;
	this->_initServer();
	OUTNL(MAGENTA("Ready to connect in: " << _address + ":" << _port));
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
	sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_fd = accept(_server_fd, (sockaddr *)&client_addr, &client_addr_len);
	if (client_fd == -1)
		if (!this->_handleAcceptError(errno))
			return false;
	return true;
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
				// request.printRequest();
				Response response(request, this->_config);
				this->_printOnClient(client_fd, response.getResponse());
			}
		}
	}
}

int	Server::_getServerFd() const
{
	return this->_server_fd;
}
