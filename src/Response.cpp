/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoretti <bmoretti@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 10:26:03 by bmoretti          #+#    #+#             */
/*   Updated: 2024/09/22 17:32:22 by bmoretti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(Request &request, const ServerConfig &config) : _request(request), _config(config)
{
	this->httpStatusMap[HttpStatus::CONTINUE] = "100 Continue";
    this->httpStatusMap[HttpStatus::OK] = "200 OK";
    this->httpStatusMap[HttpStatus::CREATED] = "201 Created";
    this->httpStatusMap[HttpStatus::NO_CONTENT] = "204 No Content";
    this->httpStatusMap[HttpStatus::MOVED_PERMANENTLY] = "301 Moved Permanently";
    this->httpStatusMap[HttpStatus::BAD_REQUEST] = "400 Bad Request";
    this->httpStatusMap[HttpStatus::FORBIDDEN] = "403 Forbidden";
    this->httpStatusMap[HttpStatus::NOT_FOUND] = "404 Not Found";
    this->httpStatusMap[HttpStatus::NOT_ALLOWED] = "405 Method Not Allowed";
    this->httpStatusMap[HttpStatus::TIMEOUT] = "408 Request Timeout";
    this->httpStatusMap[HttpStatus::CONFLICT] = "409 Conflict";
    this->httpStatusMap[HttpStatus::PAYLOAD_TOO_LARGE] = "413 Payload Too Large";
    this->httpStatusMap[HttpStatus::SERVER_ERR] = "500 Internal Server Error";
    this->httpStatusMap[HttpStatus::NOT_IMPLEMENTED] = "501 Not Implemented";
    this->httpStatusMap[HttpStatus::SERVICE_UNAVAILABLE] = "503 Service Unavailable";

	this->_status = HttpStatus::ZERO;
	this->_determineLocation();
	if (this->_checkErrors())
		return;
	this->_identifyCGI();
	if (this->_response.isCGI)
		this->_executeCgi();
	else
		this->_generateBody(this->_response.fullURI);
	this->_status = HttpStatus::SERVER_ERR;
	this->_generateStatusLine();
	this->_generateHeaders();
}

void Response::_determineLocation()
{
	t_request request = this->_request.getRequest();
	ServerConfig server = this->_config;
	for (size_t j = 0; j < server.locations.size(); j++)
	{
		if (request.uri.find(server.locations[j].path) != std::string::npos)
		{
			this->_locationConfig = &server.locations[j];
			this->_response.fullURI = "." + server.locations[j].root + request.uri;
			return;
		}
	}
	this->_locationConfig = NULL;
}

Response::~Response() {}

std::string Response::getResponse() const
{
	return this->_generateResponse();
}

void Response::_generateStatusLine()
{
	this->_response.statusLine = "HTTP/1.1 ";
	this->_response.statusLine += this->httpStatusMap[this->_status];
}

void Response::_generateHeaders()
{
	// TODO: mocking the headers
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
		OUTNL(bufferStr);
		this->_response.body = bufferStr;
		this->_response.headers["Content-Length"] = itoa(bufferStr.size());
		this->_response.headers["Content-Type"] = "text/html";
		file.close();
	}
	else
	{
		// TODO: mensagem de arquivo não encontrado
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

// TODO: implement other errors
bool Response::_checkErrors()
{
	if (this->_request.getRequest().HTTPVersion == "")
		this->_response.statusLine = "HTTP/1.1 400 Bad Request";
	if (this->_request.getRequest().method == OTHER)
		this->_error405();
	else
		return false;
	return true;
}

bool Response::_checkError400()
{
	t_request request = this->_request.getRequest();

	if (request.HTTPVersion == "")
		return true;
	if (request.headers["Host"] == "")
		return true;
	int content_length = 0;
	int body_size = 0;
	if (request.headers.find("Content-Length") != request.headers.end())
		content_length = atoi(request.headers["Content-Length"].c_str());
	if (request.headers.find("Body") != request.headers.end())
		body_size = request.headers["Body"].size();
	if (content_length != body_size)
		return true;
	if (request.method == POST && content_length == 0)
		return true;
	if (request.method == POST && request.headers["Content-Type"] == "")
		return true;
	return false;
}

bool Response::_checkError404()
{
	struct stat buffer;
	if (stat(this->_response.fullURI.c_str(), &buffer) != 0)
	{
		this->_status = HttpStatus::NOT_FOUND;
		return true;
	}
	return false;
}

// TODO: fix this function, verify where need to return true or false
bool Response::_error405()
{
	const ServerConfig &server = this->_config;
	t_request request = this->_request.getRequest();

	this->_response.statusLine = "HTTP/1.1 405 Method Not Allowed";
	if (server.server_name == trim(request.headers["Host"]))
	{
		if (server.error_pages.find(405) != server.error_pages.end())
			this->_generateBody((std::string &)server.error_pages.at(405));
		else
		{
			std::string path("/web/405.html");
			this->_generateBody(path);
		}
		return true; // verify if here is true or false
	}
	return false; // verify if it's true or false too
}

void Response::_identifyCGI()
{
	t_request request = this->_request.getRequest();
	ServerConfig server = this->_config;
	for (size_t j = 0; j < server.locations.size(); j++)
	{
		if (request.uri.find(server.locations[j].path) != std::string::npos)
		{
			for (size_t k = 0; k < server.locations[j].cgi.size(); k++)
			{
				if (request.uri.find(server.locations[j].cgi[k].extension) != std::string::npos)
				{
					this->_location = server.locations[j].root;
					this->_response.isCGI = true;
					return;
				}
			}
		}
	}
	this->_response.isCGI = false;
}

void Response::_executeCgi()
{
  std::string script_path = this->_request.getRequest().uri;
	pid_t pid = fork();
	script_path = this->_location.substr(1) + script_path;
	int fd;

	if (pid < 0)
	{
		perror("Fork failed");
		return;
	}

	if (pid == 0)
	{
		std::string executablePath;
		if (endsWith(script_path, ".py"))
		{
		if (isInterpreterInstalled("python3"))
			executablePath = PY_PATH;
		}
		else if (endsWith(script_path, ".go"))
		{
		if (isInterpreterInstalled("go"))
			executablePath = GO_PATH;
		}
		else if (endsWith(script_path, ".php"))
		{
		if (isInterpreterInstalled("php"))
			executablePath = PHP_PATH;
		}
		else
		{
			OUTNL("Not a valid CGI");
		}

		// Redirect stdout to the client socket
		fd = open("temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);

		// Execute the CGI script
		char *args[] = {const_cast<char *>(executablePath.c_str()),
					const_cast<char *>(script_path.c_str()), NULL};
		std::cerr << "Chegou aqui\n";
		execve(args[0], args, NULL);

		// If execve fails
		perror("execve failed");
		close(fd);
		exit(1);
	}
	else
	{
		// Parent process
		// Wait for the child process to finish
		waitpid(pid, NULL, 0);
		std::string path("temp");
		this->_generateBody(path);
		std::remove("temp");
	}
}
