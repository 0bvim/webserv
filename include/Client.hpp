/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumedeir < lumedeir@student.42sp.org.br    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:40:02 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/06 16:09:27 by lumedeir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "common.hpp"

class Client
{
public:
	Client();
	~Client();
	void	connectToServer(const int serverSocket);
	void	_setNonBlock(void);
	int		getClientSocket(void) const;
	int		getPortClient(void)	const;
	bool	_isConnected(void) const;
	void	_setConnected(bool status);
	void	_setClientSocket(int socket);

protected:
	bool	_connected;

private:
	int					_clientSocket;
	struct sockaddr_in	_address;

	void	_createClientSocket(void);


};

#endif
