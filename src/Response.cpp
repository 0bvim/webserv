/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:07:40 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/16 15:06:32 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request & request) : _request(request)
{
	// o request será usado para gerar o response. está (void) para compilar
	(void)this->_request;
	this->_generateStatusLine();
	this->_generateHeaders();
	this->_generateBody();
}

Response::~Response()
{
}

std::string	Response::getResponse() const
{
	return this->_generateResponse();
}

void	Response::_generateStatusLine()
{
	// mocking the status line
	this->_response.statusLine = "HTTP/1.1 200 OK";
}

void	Response::_generateHeaders()
{
	// mocking the headers
	this->_response.headers["Content-Type"] = "text/html";
}

void	Response::_generateBody()
{
	std::ifstream file("./web/index.html");

	if (file.is_open()) {
		std::stringstream	buffer;
		std::string			bufferStr;
		buffer << file.rdbuf();
		bufferStr = buffer.str();
		this->_response.body = bufferStr;
		this->_response.headers["Content-Length"] = itoa(bufferStr.size());
		file.close();
	} else {
		// [TODO] mensagem de arquivo não encontrado
	}
}

std::string	Response::_generateResponse() const
{
	std::string response = this->_response.statusLine + "\r\n";
	for (std::map<std::string, std::string>::const_iterator it =
		this->_response.headers.begin(); it != this->_response.headers.end();
		++it) {
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n" + this->_response.body;
	return response;
}
