/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-frei <vde-frei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:41 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/28 15:53:34 by vde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "common.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "Config.hpp"

class Server
{
public:
	Server(Config & config);
	~Server();

	void run();

private:
/* Server initialization */
	void	_initServer();
	void	_handleConnection(int client_fd);
	void	_handleEvents();
	void	_setNonBlocking(int fd);
	void	_fillBuffer(int fd, const char *str);

	std::string _address;
	int _port;
	int _server_fd;
	int _epoll_fd;
	std::map<int, std::string> _buffer_request;
	struct epoll_event _events[MAX_EVENTS];
	Config & _config;
};

#endif
