/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:07:40 by bmoretti          #+#    #+#             */
/*   Updated: 2024/08/03 18:13:44 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request &request, Config &config) : _request(request), _config(config)
{
	// o request será usado para gerar o response. está (void) para compilar
	(void)this->_request;
	this->_identifyCGI();
	if (this->_checkErrors())
		return;
	this->_generateStatusLine();
	this->_generateHeaders();
	std::string path("/web/index.html");
	this->_generateBody(path);
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

void Response::_generateBody(std::string &path)
{
	std::ifstream file(path.c_str());

	if (file.is_open())
	{
		std::stringstream buffer;
		std::string bufferStr;
		buffer << file.rdbuf();
		bufferStr = buffer.str();
		this->_response.body = bufferStr;
		this->_response.headers["Content-Length"] = itoa(bufferStr.size());
		this->_response.headers["Content-Type"] = "text/html";
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

void Response::_identifyCGI()
{
	t_request request = this->_request.getRequest();
	std::vector<ServerConfig> servers = this->_config.getServers();
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].server_name == trim(request.headers["Host"]))
		{
			for (size_t j = 0; j < servers[i].locations.size(); j++)
			{
				if (request.uri.find(servers[i].locations[j].path) != std::string::npos)
				{
					for (size_t k = 0; k < servers[i].locations[j].cgi.size(); k++)
					{
						if (request.uri.find(servers[i].locations[j].cgi[k].extension) != std::string::npos)
							this->_response.isCGI = true;
					}
				}
			}
		}
	}
	this->_response.isCGI = false;
}

bool Response::_checkErrors()
{
	if (this->_request.getRequest().method == OTHER)
		this->_error405();
	else
		return false;
	return true;
}

void Response::_error405()
{
	std::vector<ServerConfig> servers = this->_config.getServers();
	t_request request = this->_request.getRequest();

	this->_response.statusLine = "HTTP/1.1 405 Method Not Allowed";
	for (size_t i = 0; i < servers.size(); i++)
	{
		if (servers[i].server_name == trim(request.headers["Host"]))
		{
			if (servers[i].error_pages.find(405) != servers[i].error_pages.end()) {
				OUTNL("HERE1");
				this->_generateBody(servers[i].error_pages[405]);
			}
			else {
				OUTNL("HERE2");
				std::string path("/web/405.html");
				this->_generateBody(path);
			}
		}
	}
}
