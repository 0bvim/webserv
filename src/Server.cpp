/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumedeir < lumedeir@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:35 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/06 18:50:23 by lumedeir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "common.hpp"

static bool isSigInt = false;

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
	std::cout << "Client disconnected" << std::endl;
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

void	Server::_acceptClient(int socket) {
	Client *client = new Client();

	// client->connectToServer(this->_serverSocket);
	client->_setClientSocket(socket);
	if (client->_isConnected() == true)
		return;
	client->_setNonBlock();
	// client->_setConnected(true);
	this->_clients.push_back(client);
}

// void	Server::_verifyClients(void) {
// 	for (std::vector<Client *>::iterator it = this->_clients.begin();
// 		it != this->_clients.end(); it++) {
// 		it
// 		//do_use_fd(*it);
// 	}
// }

void	Server::_handleSignal(int)
{
	isSigInt = true;
	OUTNL(RED("\r[HALT]: STOPPING THE SERVER... \n"));
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
				conn_sock = accept(this->_serverSocket, (struct sockaddr *) &this->_address, (socklen_t*)&this->_address);
				if (conn_sock < 0) {
					throw std::runtime_error("Error accepting client");
				}
				this->_acceptClient(conn_sock);
					
				if(send(conn_sock, "Hello, world!\n", 14, 0) != -1)
					std::cout << "Mensagem enviada" << std::endl;
				else
					std::cout << "Erro ao enviar mensagem" << std::endl;
				}
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_sock;
				if (!this->_clients.back()->_isConnected() && epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
					throw std::runtime_error("Error adding client server poll");
				}
				else if (!this->_clients.back()->_isConnected()){
					this->_clients.back()->_setConnected(true);
				}
				else if (events[n].events & EPOLLIN) {
					char buffer[1024];
					ssize_t bytesRead = read(events[n].data.fd, buffer, sizeof(buffer) - 1);
					if (bytesRead > 0) {
						buffer[bytesRead] = '\0'; // Null-terminate the buffer
						std::string data(buffer);
						std::istringstream ss(data);
						std::string line;
						while (std::getline(ss, line)) {
							std::cout << "Received from client " << events[n].data.fd << " " << line << std::endl;
							if (line == "exit\r"){
								close(events[n].data.fd);
								for (std::vector<Client *>::iterator it = this->_clients.begin();
									it != this->_clients.end(); it++) {
									if ((*it)->getClientSocket() == events[n].data.fd){
										delete *it;
										this->_clients.erase(it);
										break;
									}
								}
							}
							else {
								std::string response = "You said: " + line + "\n";
								send(events[n].data.fd, response.c_str(), response.size(), 0);
							}
						}
					} else if (bytesRead == 0) {
						// Conexão fechada pelo cliente
						close(events[n].data.fd);
					} else {
						std::cerr << "Error reading from client" << std::endl;
				}
			}
		}
	}
}
