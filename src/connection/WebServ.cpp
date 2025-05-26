#include "Cgi.hpp"
#include "Connection.hpp"
#include "Http.hpp"
#include "WebServ.hpp"
#include "IStream.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "standard.hpp"
#include "step.hpp"
#include <cerrno>
#include <cstdio>
#include <exception>
#include <list>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

WebServ *WebServ::_instance = NULL;

WebServ::WebServ(void) : _epoll_fd(-1) {}

WebServ::~WebServ(void)
{
	while (_connections.begin() != _connections.end())
	{
		map<int, IStream *>::iterator ic = _connections.begin();
		closeConnection(ic->first);
	}

	map<string, int>::iterator socket = _sockets.begin();
	for (; socket != _sockets.end(); ++socket)
		if (socket->second != -1)
			close(socket->second);

	if (_epoll_fd != -1)
		close(_epoll_fd);
}

WebServ *WebServ::getInstance(void) {

	if (_instance == NULL)
		_instance = new WebServ();

	return _instance;
}

void WebServ::removeBindedPorts(const string &port)
{
	set<string> sockets_to_remove;

	map<string, int>::iterator socket = _sockets.begin();
	for (; socket != _sockets.end(); ++socket)
	{

		list<string> tmp2 = parser::split(socket->first, ':');
		if (port != tmp2.back())
			continue;

		if (socket->second != -1)
			close(socket->second);
		sockets_to_remove.insert(socket->first);
	}

	set<string>::iterator socket_to_remove = sockets_to_remove.begin();
	for(; socket_to_remove != sockets_to_remove.end(); ++socket_to_remove)
		_sockets.erase(_sockets.find(*socket_to_remove));
}

bool WebServ::isBinded(const string &host)
{
	if (_sockets.find(host) != _sockets.end())
		return true;

	list<string> host_part = parser::split(host, ':');
	if (_sockets.find("0.0.0.0:" + host_part.back()) != _sockets.end())
		return true;

	if (host_part.front() != "0.0.0.0")
		return false;

	removeBindedPorts(host_part.back());

	return false;
}

struct addrinfo *WebServ::getAddrInfo(const string &host)
{
	struct addrinfo addrinfo_config = (struct addrinfo){};
	struct addrinfo *addrinfo_result = NULL;

	addrinfo_config.ai_family = AF_INET;
	addrinfo_config.ai_socktype = SOCK_STREAM;
	addrinfo_config.ai_flags = AI_PASSIVE;

	list<string> host_part = parser::split(host, ':');

	const char *ip = host_part.front().c_str();
	const char *port = host_part.back().c_str();

	if (getaddrinfo(ip, port, &addrinfo_config, &addrinfo_result))
		throw runtime_error("getaddrinfo failed");

	return addrinfo_result;
}

int WebServ::createSocket(const string &host)
{
	struct addrinfo *addrinfo_result = getAddrInfo(host);

	const int socket_fd = socket(
		addrinfo_result->ai_family,
		addrinfo_result->ai_socktype,
		addrinfo_result->ai_protocol
	);
	if (socket_fd == -1)
		throw runtime_error("failed to create socket on: " + host);

	int option = 1;

	const int setsockopt_status = setsockopt(
		socket_fd,
		SOL_SOCKET,
		SO_REUSEADDR,
		&option,
		sizeof(option)
	);
	if (setsockopt_status == -1)
	{
		close(socket_fd);
		freeaddrinfo(addrinfo_result);

		throw runtime_error("failed to set socket option on: " + host);
	}

	const int bind_status = bind(
		socket_fd,
		addrinfo_result->ai_addr,
		addrinfo_result->ai_addrlen
	);
	if (bind_status == -1)
	{
		close(socket_fd);
		freeaddrinfo(addrinfo_result);

		throw runtime_error("failed to bind on: " + host);
	}

	freeaddrinfo(addrinfo_result);

	const int listen_result = listen(socket_fd, standard::MAX_EVENTS);
	if (listen_result == -1)
	{
		close(socket_fd);

		throw runtime_error("failed to listen on: " + host);
	}

	return socket_fd;
}

void WebServ::controlEpoll(const int &socket_fd, const int &flag, const int &option)
{
	if (_epoll_fd == -1 || socket_fd == -1)
		return;

	struct epoll_event event = (struct epoll_event){};
	event.events = flag;
	event.data.fd = socket_fd;

	const int epoll_ctl_status = epoll_ctl(
		_epoll_fd,
		option,
		socket_fd,
		&event
	);
	if (epoll_ctl_status == 0)
		return;

	stringstream ss;
	ss << "failed to configure epoll_ctl for socket: " << socket_fd;

	logger::error(ss.str());
}

static string getIpByAddr(struct sockaddr_in &addr)
{
	const int ip = htonl(addr.sin_addr.s_addr);

	stringstream ss;
	ss << ((ip & 0xFF000000) >> 24) << ".";
	ss << ((ip & 0x00FF0000) >> 16) << ".";
	ss << ((ip & 0x0000FF00) >> 8) << ".";
	ss << (ip & 0x000000FF) << ":";
	ss << htons(addr.sin_port);

	return ss.str();
}

string WebServ::getIpByFileDescriptor(const int &socket_fd)
{
	struct sockaddr_in addr = (sockaddr_in){};
	socklen_t addr_len = sizeof(addr);

	const int getsockname_status = getsockname(
		socket_fd,
		(struct sockaddr *) &addr,
		&addr_len
	);
	if (getsockname_status == -1)
	{
		stringstream ss;
		ss << "failed to getsockname for socket: " << socket_fd;

		logger::error(ss.str());
	}

	return getIpByAddr(addr);
}

void WebServ::acceptNewConnection(const int &socket_fd)
{
	struct sockaddr_in client_addr = (sockaddr_in){};
	socklen_t client_len = sizeof(client_addr);

	const int client_socket_fd = accept(
		socket_fd,
		(struct sockaddr *)&client_addr,
		&client_len
	);
	if (client_socket_fd == -1)
		return logger::fatal("accept failed");

	const string host_ip = getIpByFileDescriptor(client_socket_fd);
	const string client_ip = getIpByAddr(client_addr);

	logger::debug(client_ip + " connection accepted");
	controlEpoll(client_socket_fd, EPOLLIN | EPOLLET, EPOLL_CTL_ADD);

	Connection *connection = new Connection(client_socket_fd, host_ip);
	connection->setIp(client_ip);

	_connections[client_socket_fd] = connection;
}

void WebServ::closeConnection(const int &socket_fd)
{
	map<const int, IStream *>::iterator connection;
	connection = _connections.find(socket_fd);
	if (connection == _connections.end())
		return;

	if (dynamic_cast<Connection *>(connection->second))
	{
		controlEpoll(socket_fd, 0, EPOLL_CTL_DEL);
		logger::debug(connection->second->getIp() + " connection closed");

		if (connection->first != -1)
			close(connection->first);
		delete dynamic_cast<Connection *>(connection->second);
	}
	else if (dynamic_cast<Cgi *>(connection->second))
		dynamic_cast<Cgi *>(connection->second)->sendCGI();

	_connections.erase(connection);
}

void WebServ::readFailed(IStream *connection)
{
	if (dynamic_cast<Connection *>(connection))
		logger::error("failed to read from client: " + connection->getId());

	closeConnection(connection->getFd());
}

void WebServ::readNoBytes(IStream *connection)
{
	Cgi *cgi = dynamic_cast<Cgi *>(connection);
	if (cgi != NULL)
		return cgi->sendCGI();

	logger::warning(connection->getIp() + " disconected");
	closeConnection(connection->getFd());
}

void WebServ::readUnexpectedEOF(IStream *connection)
{
	logger::warning(connection->getIp() + " interrupted");
	closeConnection(connection->getFd());
}

void WebServ::inputHandler(map<int, IStream *>::iterator &stream) {

	const int socket_fd = stream->first;
	IStream *connection = stream->second;

	vector<char> buffer(standard::BUFFER_SIZE);

	int bytes_read = recv(
		socket_fd,
		buffer.data(),
		standard::BUFFER_SIZE,
		MSG_NOSIGNAL
	);
	if (bytes_read == -1)
		return readFailed(connection);
	else if (bytes_read == 0)
		return readNoBytes(connection);
	else if (buffer.at(0) == EOF && connection->getStep() < step::BODY)
		return readUnexpectedEOF(connection);

	connection->setData(buffer, bytes_read);

	if (connection->getStep() < step::BODY)
		return controlEpoll(socket_fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);

	if (dynamic_cast<Connection *>(connection))
		return controlEpoll(socket_fd, EPOLLIN | EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);
}

void WebServ::sendFailed(IStream *connection)
{
	if (dynamic_cast<Connection *>(connection))
		logger::fatal(connection->getIp() + "client is no longer available to receive messages");

	closeConnection(connection->getFd());
}

void WebServ::sendNoBytes(IStream *connection)
{
	const size_t socket_fd = connection->getFd();

	if (dynamic_cast<Cgi *>(connection))
		return controlEpoll(socket_fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);

	if (connection->getStep() == step::CLOSE)
		return closeConnection(socket_fd);

	if (connection->getStep() == step::KEEPALIVE)
	{
		dynamic_cast<Connection *>(connection)->resetConnection();
		return controlEpoll(socket_fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);
	}
}

void WebServ::outputHandler(map<int, IStream *>::iterator &stream)
{
	const int socket_fd = stream->first;
	IStream *connection = stream->second;

	if (connection->getStep() < step::RESPONSE)
		return controlEpoll(socket_fd, EPOLLIN | EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);

	const string data = connection->getData(standard::BUFFER_SIZE);

	int bytes_send = send(socket_fd, data.c_str(), data.size(), MSG_NOSIGNAL);
	if (bytes_send == -1)
		return sendFailed(connection);
	else if (bytes_send == 0)
		return sendNoBytes(connection);

	return controlEpoll(socket_fd, EPOLLIN | EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);
}

void WebServ::checkTimeOut(void)
{
	if (_connections.empty())
		return;

	map<int, IStream *>::iterator stream = _connections.begin();
	for (; stream!= _connections.end(); ++stream)
	{
		Connection *connection = dynamic_cast<Connection *>(stream->second);
		if (!connection)
			continue;

		const int socket_fd = stream->first;

		if (connection->isTimedOut())
		{
			if (connection->getStep() != step::NONE)
				return connection->sendTimeOut();

			closeConnection(socket_fd);
			break;
		}

		if (!connection->isKeepAliveTimedOut())
			continue;

		closeConnection(socket_fd);
		break;
	}
}

void WebServ::connectHosts(void)
{
	vector<Server> servers = Http::getInstance()->getServers();

	vector<Server>::iterator server = servers.begin();
	for (; server != servers.end(); ++server)
	{
		vector<string> hosts = server->getListen();
		vector<string>::iterator host = hosts.begin();
		for (; host != hosts.end(); ++host)
		{
			if (isBinded(*host))
				continue;

			_sockets[*host] = createSocket(*host);
		}
	}
}

void WebServ::createEpoll(void)
{
	_epoll_fd = epoll_create(1);
	if (_epoll_fd < 0)
		throw runtime_error("failed to create an epoll");

	map<string, int>::iterator socket = _sockets.begin();
	for (; socket != _sockets.end(); ++socket)
	{
		controlEpoll(socket->second, EPOLLIN, EPOLL_CTL_ADD);
		logger::debug("server started, listening on " + socket->first);
	}
}

void WebServ::checkEvents(const int &num_events, epoll_event *events)
{
	for (int i = 0; i < num_events; ++i)
	{
		int socket_fd = events[i].data.fd;

		try
		{
			map<int, IStream *>::iterator stream = _connections.find(socket_fd);
			if (stream == _connections.end())
				acceptNewConnection(socket_fd);
			else if (events[i].events & (EPOLLIN | EPOLLET))
				inputHandler(stream);
			else if (events[i].events & (EPOLLOUT | EPOLLET))
				outputHandler(stream);
		}
		catch (exception &e)
		{
			string error = "disconecting client by: ";
			logger::error(error + e.what());
			closeConnection(socket_fd);
		}
	}
}

void WebServ::run(void)
{
	connectHosts();
	createEpoll();

	epoll_event events[standard::MAX_EVENTS];

	while (true)
	{
		int num_events = epoll_wait(
			_epoll_fd,
			events,
			standard::MAX_EVENTS,
			1000
		);
		if (num_events == -1)
			return logger::fatal("server was shooting down");

		checkEvents(num_events, events);
		checkTimeOut();
	}
}

void WebServ::stop(void)
{
	if (_epoll_fd == -1)
		return ;

	close(_epoll_fd);
	_epoll_fd = -1;
}

void WebServ::addStream(IStream *stream)
{
	_connections[stream->getFd()] = stream;
}

void WebServ::delStream(const int &socket_fd)
{
	map<int, IStream *>::iterator stream = _connections.find(socket_fd);
	if (stream == _connections.end())
		return;

	_connections.erase(stream);
}
