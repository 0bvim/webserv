/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:46:10 by bmoretti          #+#    #+#             */
/*   Updated: 2024/06/27 15:05:20 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {
}

Client::~Client() {
	close(this->_clientSocket);
}

void	Client::connectToServer(const int serverSocket) {
	socklen_t	addressSize = sizeof(this->_address);

	this->_clientSocket =  accept(serverSocket,
		(struct sockaddr *)&this->_address, &addressSize);
	if (this->_clientSocket == -1) {
		throw std::runtime_error("Error connecting to the server");
	}
}

void	Client::_createClientSocket(void) {
	this->_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_clientSocket == -1) {
		throw std::runtime_error("Error creating the client socket");
	}
}

