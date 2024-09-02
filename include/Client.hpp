/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:40:02 by bmoretti          #+#    #+#             */
/*   Updated: 2024/09/01 16:30:10 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "common.hpp"

class Client
{
public:
	Client(int serverSocket);
	~Client();
	int _getClientSocket() const;
	std::string _getBuffer();
	void _addToBuffer(const std::string &str);

private:
	int _clientSocket;
	struct sockaddr_in _address;
	std::string _buffer;
};

#endif
