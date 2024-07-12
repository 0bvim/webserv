/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:29:18 by bmoretti          #+#    #+#             */
/*   Updated: 2024/07/11 19:00:26 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(const char * str) : _str(str)
{
	this->_parseHTTPRequest();
}

Parser::~Parser()
{

}

t_request	Parser::getRequest() const
{
	return this->_request;
}

std::string	Parser::_trim(const std::string & str)
{
	std::string::size_type	first = str.find_first_not_of(' ');
	std::string::size_type	last = str.find_last_not_of(' ');

	if (first == std::string::npos || last == std::string::npos) {
		return "";
	}
	return str.substr(first, last - first + 1);
}

void	Parser::_parseHTTPRequest()
{
	std::istringstream	requestStream(this->_str);
	std::string			line;

	std::getline(requestStream, line);
	std::istringstream	requestLineStream(line);
	requestLineStream >> this->_request.method;
	requestLineStream >> this->_request.uri;

	while (std::getline(requestStream, line) && line != "\r") {
		std::string::size_type pos = line.find(':');
		if (pos != std::string::npos) {
			std::string	key = this->_trim(line.substr(0, pos));
			std::string	value = this->_trim(line.substr(pos + 1));
			this->_request.headers[key] = value;
		}
	}
}
