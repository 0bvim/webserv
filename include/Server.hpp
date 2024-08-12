/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nivicius <nivicius@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:41 by bmoretti          #+#    #+#             */
/*   Updated: 2024/08/12 20:27:30 by nivicius         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "common.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"

class Server
{
public:
	Server(ServerConfig &config);
	~Server();
	void run();

private:
	/* Server initialization */
	void _initServer();
	void _handleConnection(int client_fd);
	void _handleEvents();
	void _setNonBlocking(int fd);
	void _fillBuffer(int fd, const char *str);
	bool _checkEndMessage(int fd);
	void _printOnClient(int fd, std::string const &str);
	ssize_t _readFromClient(int fd, char *buff);
	bool _handleAcceptError(int error_code);
	void _initServerAddress(sockaddr_in &server_add);
	bool _acceptClient();

	std::string _address;
	int _port;
	int _server_fd;
	int _epoll_fd;
	HttpStatus::Code _status;
	std::map<int, std::string> _buffer_request;
	struct epoll_event _events[MAX_EVENTS];
	ServerConfig &_config;
};

#endif
