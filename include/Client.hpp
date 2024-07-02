/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:40:02 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/01 13:47:27 by bmoretti         ###   ########.fr       */
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
	int		getClientSocket(void) const;

private:
	int					_clientSocket;
	struct sockaddr_in	_address;

	void	_createClientSocket(void);
	void	_setNonBlock(void);

};

#endif
