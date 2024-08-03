/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:07:40 by bmoretti          #+#    #+#             */
/*   Updated: 2024/08/03 15:03:04 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request &request, Config &config) : _request(request), _config(config)
{
	// o request será usado para gerar o response. está (void) para compilar
	(void)this->_request;
	this->_identifyCGI();
	this->_generateStatusLine();
	this->_generateHeaders();
	this->_generateBody();
}

Response::~Response()
{
}

std::string Response::getResponse() const
{
	return this->_generateResponse();
}

void Response::_generateStatusLine()
{
	// mocking the status line
	this->_response.statusLine = "HTTP/1.1 200 OK";
}

void Response::_generateHeaders()
{
	// mocking the headers
	this->_response.headers["Content-Type"] = "text/html";
}

void Response::_generateBody()
{
	std::ifstream file("./web/index.html");

	if (file.is_open())
	{
		std::stringstream buffer;
		std::string bufferStr;
		buffer << file.rdbuf();
		bufferStr = buffer.str();
		this->_response.body = bufferStr;
		this->_response.headers["Content-Length"] = itoa(bufferStr.size());
		file.close();
	}
	else
	{
		// [TODO] mensagem de arquivo não encontrado
	}
}

std::string Response::_generateResponse() const
{
	std::string response = this->_response.statusLine + "\r\n";
	for (std::map<std::string, std::string>::const_iterator it =
			 this->_response.headers.begin();
		 it != this->_response.headers.end();
		 ++it)
	{
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n" + this->_response.body;
	return response;
}

bool Response::_identifyCGI()
{
	t_request request = this->_request.getRequest();
	std::vector<ServerConfig> servers = this->_config.getServers();
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].server_name == trim(request.headers["Host"]))
		{
			for (size_t j = 0; j < servers[i].locations.size(); j++)
			{
				OUTNL(servers[i].locations[j].path);
				if (request.uri.find(servers[i].locations[j].path) != std::string::npos)
				{
					OUTNL("ENTROU");
					for (size_t k = 0; k < servers[i].locations[j].cgi.size(); k++)
					{
						OUTNL(servers[i].locations[j].cgi[k].extension);
						if (request.uri.find(servers[i].locations[j].cgi[k].extension) != std::string::npos)
						{
							std::cout << "CGI found" << std::endl;
							return true;
						}
					}
				}
			}
		}
	}
	std::cout << "CGI not found" << std::endl;
	return false;
}
