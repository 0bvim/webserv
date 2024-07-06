/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumedeir < lumedeir@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:41 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/16 14:54:14 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "common.hpp"
# include "Request.hpp"
# include "Response.hpp"

class Server
{
public:
	Server(const std::string &address, int port);
	~Server();

	void run();

private:
/* Server initialization */
	void	_initServer();
	void	_handleConnection(int client_fd);
	void	_handleEvents();
	void	_setNonBlocking(int fd);

	std::string _address;
	int _port;
	int _server_fd;
	int _epoll_fd;
	struct epoll_event _events[MAX_EVENTS];
};

#endif
