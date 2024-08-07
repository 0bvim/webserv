/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nivicius <nivicius@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:07:40 by bmoretti          #+#    #+#             */
/*   Updated: 2024/08/04 21:06:24 by nivicius         ###   ########.fr       */
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
	if (this->_response.isCGI)
		this->_executeCgi();
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
	for (size_t i = servers.size() - 1; i != std::string::npos; --i)
	{
		if (servers[i].server_name == trim(request.headers["Host"]) || i == 0)
		{
			if (servers[i].error_pages.find(405) != servers[i].error_pages.end())
				this->_generateBody(servers[i].error_pages[405]);
			else {
				std::string path("/web/405.html");
				this->_generateBody(path);
			}
		}
	}
}

void Response::_identifyCGI()
{
	t_request request = this->_request.getRequest();
	std::vector<ServerConfig> servers = this->_config.getServers();
	for (size_t i = servers.size() - 1; i != std::string::npos; --i)
	{
		if (servers[i].server_name == trim(request.headers["Host"]) || i == 0) {
			for (size_t j = 0; j < servers[i].locations.size(); j++) {
				if (request.uri.find(servers[i].locations[j].path) != std::string::npos) {
					for (size_t k = 0; k < servers[i].locations[j].cgi.size(); k++)
						if (request.uri.find(servers[i].locations[j].cgi[k].extension) != std::string::npos) {
							this->_response.isCGI = true;
							return;
						}
				}
			}
		}
	}
	this->_response.isCGI = false;
}

void Response::_executeCgi()
{
	pid_t pid = fork();
	std::string script_path = this->_request.getRequest().uri;
	int fd;
	
	
    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) {
        // Set up the environment variables (if needed)
        char *env[] = { NULL };

        // Redirect stdout to the client socket
		fd = open("temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);

        // Execute the CGI script
		char *args[] = { const_cast<char*>("/usr/bin/python3"), const_cast<char*>(script_path.c_str()), NULL };
		execve(const_cast<char*>("/usr/bin/python3"), args, env);

        // If execve fails
        perror("execve failed");
        close(fd);
        exit(1);
    } else {
        // Parent process
        // Wait for the child process to finish
        waitpid(pid, NULL, 0);
		std::string path("temp");
		this->_generateBody(path);
		std::remove("temp");
    }
}
