/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumedeir < lumedeir@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:41 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/06 18:40:23 by lumedeir         ###   ########.fr       */
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
	bool						_isSigInt;
	struct sockaddr_in			_address;
	std::vector<Client *>		_clients;

// Server initialization
	void	_createServerSocket(void);
	void	_bindServer(void);
	void	_listen(void);
	void	_polling(void);
	void	_verifyClients(void);
	static void	_handleSignal(int c);

// Client management
	void	_acceptClient(int socket);

};

#endif
