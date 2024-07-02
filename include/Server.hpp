/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:41 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/01 13:13:04 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "common.hpp"

class Server {
public:
	Server();
	~Server();

private:
	int							_serverSocket;
	struct sockaddr_in			_address;
	std::vector<Client *>		_clients;
	std::vector<struct pollfd>	_fds;

// Server initialization
	void	_createServerSocket(void);
	void	_bindServer(void);
	void	_listen(void);

// Client management
	void	_acceptClient(void);

// Polling
	void	_polling(void);

};

#endif
