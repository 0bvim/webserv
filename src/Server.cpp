/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:35 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/02 11:52:58 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// PUBLIC FUNCTIONS

Server::Server() {
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(PORT);
	this->_createServerSocket();
	this->_bindServer();
	this->_listen();
	this->_polling();
}

Server::~Server() {
	for (std::vector<Client *>::iterator it = this->_clients.begin();
		it != this->_clients.end(); it++) {
	delete *it;
	}
	close(this->_serverSocket);
}

// PRIVATE FUNCTIONS

void	Server::_createServerSocket(void) {
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverSocket == -1) {
		throw std::runtime_error("Error creating the server socket");
	}
}

void	Server::_bindServer(void) {
	if (bind(this->_serverSocket, (struct sockaddr *)&this->_address,
			sizeof(this->_address)) == -1) {
	throw std::runtime_error("Error binding the server");
	}
}

void	Server::_listen(void) {
	if (listen(this->_serverSocket, MAX_CONNECTIONS) == -1) {
		throw std::runtime_error("Error listening the server");
	}
}

void	Server::_acceptClient(void) {
	Client *client = new Client();

	client->connectToServer(this->_serverSocket);
	this->_clients.push_back(client);
}

void	Server::_polling(void) {
	struct epoll_event ev, events[MAX_EVENTS];
	int conn_sock, nfds, epollfd;

	epollfd = epoll_create1(0);
	if (epollfd == -1) {
		throw std::runtime_error("Error polling");
	}
	ev.events = EPOLLIN;
	ev.data.fd = this->_serverSocket;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, this->_serverSocket, &ev) == -1) {
		throw std::runtime_error("Error polling the server socket");
	}
	for (;;) {
		nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			throw std::runtime_error("Error polling events");
		}
		for (int n = 0; n < nfds; ++n) {
			if (events[n].data.fd == this->_serverSocket) {
				this->_acceptClient();
				conn_sock = this->_clients.back()->getClientSocket();
			}
			ev.events = EPOLLIN | EPOLLET;
			ev.data.fd = conn_sock;
			if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
				throw std::runtime_error("Error adding client to server poll");
			}
			else {
				//RESOLVER OS EVENTOS DO NOSSO SERVER
				std::cout << "teste" << std::endl;
				//do_use_fd(events[n].data.fd);
			}
		}
	}
}
