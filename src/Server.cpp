/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumedeir < lumedeir@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:35 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/27 19:05:46 by lumedeir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(Config &config) : _config(config)
{
	this->_address = config.getServerAddress();
	this->_port = config.getServerPort();
	this->_server_fd = -1;
	this->_epoll_fd = -1;
	this->_initServer();
}

Server::~Server()
{
	if (_server_fd != -1)
		close(_server_fd);
	if (_epoll_fd != -1)
		close(_epoll_fd);
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
				while (true)
				{
					sockaddr_in client_addr;
					socklen_t client_addr_len = sizeof(client_addr);
					int client_fd = accept(_server_fd, (sockaddr *)&client_addr, &client_addr_len);
					if (client_fd == -1)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
							break;
						else
						{
							std::cerr << "Failed to accept client connection" << std::endl;
							break;
						}
					}
					this->_setNonBlocking(client_fd);
					epoll_event event;
					event.events = EPOLLIN | EPOLLET;
					event.data.fd = client_fd;
					if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
					{
						std::cerr << "Failed to add client socket to epoll" << std::endl;
						close(client_fd);
					}
				}
			}
			else
				this->_handleConnection(_events[i].data.fd); // Se o evento nao for do servidor
		}
	}
}

void Server::_handleConnection(int client_fd)
{
	char buffer[BUFFER_SIZE];
	while (true)
	{
		ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
		if (bytes_read == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break;
			else
			{
				std::cerr << "Read error" << std::endl;
				close(client_fd);
				break;
			}
		}
		else if (bytes_read == 0)
		{
			close(client_fd);
			break;
		}
		else
		{
			buffer[bytes_read] = '\0';
			Request request(buffer);
			request.printRequest();
			Response response(request, this->_config);
			std::string responseStr = response.getResponse();
			const char *respStr = responseStr.c_str();
			ssize_t bytes_written = write(client_fd, respStr, strlen(respStr));
			if (bytes_written == -1)
				std::cerr << "Write error" << std::endl;
		}
	}
}
void Server::_initServer()
{
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_server_fd == -1)
	{
		throw std::runtime_error("Failed to create socket");
	}
	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("Failed to set socket options");
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(_address.c_str());
	server_addr.sin_port = htons(_port);
	if (bind(_server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		throw std::runtime_error("Failed to bind socket");
	if (listen(_server_fd, MAX_CONNECTIONS) == -1)
		throw std::runtime_error("Failed to listen socket");
	this->_setNonBlocking(this->_server_fd);
	this->_epoll_fd = epoll_create1(0);
	if (this->_epoll_fd == -1)
		throw std::runtime_error("Failed to create epoll file descriptor");
	epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = this->_server_fd;
	if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _server_fd, &event) == -1)
		throw std::runtime_error("Failed to add server socket to epoll");
}

void Server::_setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw std::runtime_error("Failed to get file descriptor flags");
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw std::runtime_error("Failed to set non-blocking mode");
}
