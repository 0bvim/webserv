/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nivicius <nivicius@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:46:10 by bmoretti          #+#    #+#             */
/*   Updated: 2024/08/08 01:55:14 by nivicius         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client() {}

Client::~Client()
{
	close(this->_clientSocket);
}

void Client::connectToServer(const int serverSocket)
{
	socklen_t addressSize = sizeof(this->_address);

	this->_clientSocket = accept(serverSocket,
								 (struct sockaddr *)&this->_address, &addressSize);
	if (this->_clientSocket == -1)
		throw std::runtime_error("Error connecting to the server");
}

int Client::getClientSocket(void) const
{
	return this->_clientSocket;
}

void Client::_createClientSocket(void)
{
	this->_clientSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (this->_clientSocket == -1)
	{
		throw std::runtime_error("Error creating the client socket");
	}
}
