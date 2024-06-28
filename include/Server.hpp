/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 10:40:41 by bmoretti          #+#    #+#             */
/*   Updated: 2024/06/27 15:04:14 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "common.hpp"
# include "Client.hpp"

class Server
{
public:
	Server();
	~Server();

private:
	int						_serverSocket;
	struct sockaddr_in		_address;
	std::vector<Client*>	_clients;

	void	_createServerSocket(void);
	void	_bindServer(void);
	void	_listen(void);
	void	_acceptClient(void);

};

#endif
