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
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		delete it->second;
		_clients.erase(it);
		break;
	}
}

bool Server::_acceptClient()
{
	Client *client = new Client(this->_server_fd);

	int client_fd = client->_getClientSocket();

	(*_event).events = EPOLLIN | EPOLLET;
	(*_event).data.fd = client_fd;
	if (epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, client_fd, _event) == -1)
	{
		perror("epoll_ctl failed");
		close(client_fd);
	}
	this->_clients[client_fd] = client;
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

// void Server::_handleConnection(int client_fd)
// {
// 	char buffer[BUFFER_SIZE];

// 	while (true)
// 	{
// 		ssize_t bytes_read = _readFromClient(client_fd, buffer);
// 		if (bytes_read <= 0)
// 			break;
// 		else
// 		{
// 			buffer[bytes_read] = '\0';
// 			_fillBuffer(client_fd, buffer);
// 			if (this->_status == HttpStatus::BAD_REQUEST)
// 			{
// 				this->_printOnClient(client_fd, "Bad Request\n");
// 				close(client_fd);
// 			}
// 			else if (_checkEndMessage(client_fd))
// 			{
// 				Request request(this->_buffer_request[client_fd].c_str());
// 				// request.printRequest();
// 				Response response(request, this->_config);
// 				this->_printOnClient(client_fd, response.getResponse());
// 			}
// 		}
// 	}
// }

bool endsWithCRLF(const std::string &str)
{
	const std::string crlf = "\r\n\r\n";
	if (str.length() >= crlf.length())
	{
		return str.substr(str.length() - crlf.length()) == crlf;
	}
	return false;
}

void Server::_handleConnection(int client_fd)
{
	char buffer[BUFFER_SIZE + 1];

	ssize_t bytes_read = _readFromClient(client_fd, buffer);
	if (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		this->_clients[client_fd]->_addToBuffer(buffer);
	}
	if (endsWithCRLF(this->_clients[client_fd]->_getBuffer()))
		close(client_fd);
}

void Server::_setSocketNonBlocking(int fd)
{
	int flags = 1;
	if (ioctl(fd, FIONBIO, &flags) < 0)
		throw std::runtime_error("Failed to set socket non-blocking");
}

bool Server::_isClientConnected(int fd)
{
	return this->_clients.find(fd) != this->_clients.end();
}

int Server::_getServerFd() const
{
	return this->_server_fd;
}
