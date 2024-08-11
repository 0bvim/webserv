/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vde-frei <vde-frei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:40:02 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/28 22:34:05 by vde-frei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "common.hpp"

class Client
{
public:
	Client();
	~Client();
	void connectToServer(const int serverSocket);
	int getClientSocket(void) const;

private:
	int _clientSocket;
	struct sockaddr_in _address;

	void _createClientSocket(void);
};

#endif
