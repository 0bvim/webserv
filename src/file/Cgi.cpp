#include "Cgi.hpp"
#include "Connection.hpp"
#include "Environment.hpp"
#include "Http.hpp"
#include "WebServ.hpp"
#include "code.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "response.hpp"
#include "standard.hpp"
#include "step.hpp"
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "logger.hpp"
using namespace std;

Cgi::Cgi(Connection *connection) : Resource(connection), _status(0), _pid(-1)
{
	connection->setCode("");

	_sock[0] = -1;
	_sock[1] = -1;
	std::memset(_sock, EOF, 2 * sizeof(int));

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, _sock) == -1)
		return;

	_fd = _sock[1];

	_pid = fork();
	if (_pid < 0)
	{
		closeSockets();
		return;
	}
	else if (_pid == 0)
	{
		Http::getInstance()->stop(EXIT_SUCCESS);

		dup2(_sock[0], STDOUT_FILENO);
		dup2(_sock[0], STDIN_FILENO);
		closeSockets();

		string fastcgi = connection->getLocation().getFastCgi().c_str();
		string script = connection->getUri()->getAbsolutePath();

		vector<char *> argv;
		argv.push_back(const_cast<char *>(fastcgi.c_str()));
		argv.push_back(const_cast<char *>(script.c_str()));
		argv.push_back(NULL);

		Environment envp(connection);

		execve(argv.data()[0], argv.data(), envp.getEnvironment().data());

		throw runtime_error("execve failed");
	}
	close(_sock[0]);
	_sock[0] = -1;

	_output = connection->getBody();
	_step = step::RESPONSE;

	WebServ *webserv = WebServ::getInstance();
	webserv->addStream(this);
	if (_output.empty())
		webserv->controlEpoll(_fd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD);
	else
		webserv->controlEpoll(_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_ADD);
}

Cgi::Cgi(const Cgi &src) : Resource(src._connection)
{
	*this = src;
}

Cgi &Cgi::operator=(const Cgi &rhs)
{
	if (this == &rhs)
		return *this;

	return *this;
}

Cgi::~Cgi(void)
{
	if (_pid != -1)
		kill(_pid, SIGKILL);

	closeSockets();
}

void Cgi::closeSockets(void) {

	if (_sock[0] > -1)
		close(_sock[0]);

	if (_sock[1] > -1)
		close(_sock[1]);
}

void Cgi::parseCgiResponse(void)
{
	istringstream iss(_output);
	string line;

	_connection->setHeaders(standard::EMPTY_HEADER);

	while(getline(iss, line) && !line.empty())
	{
		_output.erase(0, line.size() + 1);

		if (line == "\r")
			break;

		size_t separator = line.find_first_of(":");
		if (separator == string::npos)
			return response::builder(_connection, code::INTERNAL_SERVER_ERROR);

		string key = line.substr(0, separator);
		string value = line.substr(separator + 1);

		parser::trim(value, " \t\v\r");

		_connection->addHeader(key, value);
	}
}

void Cgi::sendCGI(void)
{
	WebServ *webserv = WebServ::getInstance();
	webserv->controlEpoll(_fd, 0, EPOLL_CTL_DEL);

	if (WIFEXITED(_status) && WEXITSTATUS(_status) != 0)
		return response::builder(_connection, code::INTERNAL_SERVER_ERROR);

	if (_output.find_first_of("\r\n\r\n") == string::npos)
		return response::builder(_connection, code::INTERNAL_SERVER_ERROR);

	parseCgiResponse();

	if (*_connection == header::STATUS)
	{
		string status = (*_connection)[header::STATUS];

		parser::trim(status, " \t\v\r");

		size_t pos = status.find_first_of(" ");
		if (pos != string::npos)
		{
			_connection->setCode(status.substr(0, pos));
			_connection->setStatus(status.substr(pos + 1));
		}
	}

	if (_connection->getCode().empty())
		_connection->setCode(code::OK);
	_size = _output.size();
	_step = step::CLOSE;
	_connection->buildResponse();
	_pid = -1;

}

void Cgi::processInput(const size_t &bytes)
{
	(void) bytes;

	_output.append(_input);
	_input.erase();

	if (waitpid(_pid, &_status, WNOHANG))
		sendCGI();

	_connection->updateTime();
}
