/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:46:10 by bmoretti          #+#    #+#             */
/*   Updated: 2024/09/01 16:30:34 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(int serverSocket)
{
	memset(&this->_address, 0, sizeof(this->_address));

	socklen_t client_addr_len = sizeof(this->_address);
	this->_clientSocket = accept(serverSocket, (sockaddr *)&this->_address, &client_addr_len);
	if (this->_clientSocket == -1)
		throw std::runtime_error("Failed to accept client");
	int flags = 1;
	if (ioctl(this->_clientSocket, FIONBIO, &flags) < 0)
		throw std::runtime_error("Failed to set socket non-blocking");
}

Client::~Client()
{
	close(this->_clientSocket);
}

int Client::_getClientSocket() const
{
	return this->_clientSocket;
}

std::string Client::_getBuffer()
{
	return this->_buffer;
}
void	Client::_addToBuffer(const std::string &str)
{
	this->_buffer += str;
}
