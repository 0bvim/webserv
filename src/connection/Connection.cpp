#include "Cgi.hpp"
#include "Connection.hpp"
#include "Resource.hpp"
#include "URL.hpp"
#include "WebServ.hpp"
#include "code.hpp"
#include "header.hpp"
#include "request.hpp"
#include "response.hpp"
#include "parser.hpp"
#include "standard.hpp"
#include "step.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <ctime>

using namespace std;

Connection::Connection(const int &fd, const string &ip)
	: AStream(fd, ip),
	  _uri(NULL),
	  _file(NULL) {}

Connection::Connection(const Connection &src)
	: AStream(src),
	  _uri(NULL),
	  _file(NULL)
{
	*this = src;
}

Connection &Connection::operator=(const Connection &rhs)
{
	if (this == &rhs)
		return *this;

	_host = rhs._host;
	_method = rhs._method;
	_target = rhs._target;
	_protocol = rhs._protocol;
	_code = rhs._code;
	_status = rhs._status;
	_headers = rhs._headers;
	_body = rhs._body;
	if (_uri)
		delete _uri;
	_uri = rhs._uri;
	if (_file)
		delete _file;
	_file = rhs._file;
	_server = rhs._server;

	return *this;
}

Connection::~Connection(void)
{
	if (_uri)
		delete _uri;

	for (size_t i = 0; i < _garbage.size(); i++)
		delete _garbage[i];
}

void Connection::parseRequest(void)
{
	istringstream iss(_input);
	string line;

	while (getline(iss, line) && !line.empty() && _code.empty())
	{
		if (_step == step::HEADERS)
			return request::parseRequest(this, _input);

		if (parser::lastCharacter(line) != '\r')
			return;

		request::parseRequest(this, line);

		size_t pos = _input.find("\r\n");
		if (pos != string::npos)
			_input = _input.substr(pos + 2);

		if (_code == code::OK && _input.size())
			return response::builder(this, code::BAD_REQUEST);
	}

	if (_code.empty())
		return;

	_input.clear();
}

void Connection::setHost(const string &host)
{
	_host = host;
}

string Connection::getHost(void) const
{
	return _host;
}

void Connection::processInput(const size_t &bytes)
{
	(void) bytes;

	if (_input.find("\r\n") != string::npos || _step == step::HEADERS)
		parseRequest();

	if (_step >= step::HEADERS)
		return;

	if (_input.find("\r") != string::npos || _input.find("\n") != string::npos)
		return response::builder(this, code::BAD_REQUEST);
}

void Connection::setMethod(const string &method)
{
	_method = method;
}

string Connection::getMethod(void) const
{
	return _method;
}

void Connection::setUri(URL *uri)
{
	_uri = uri;
}

URL *Connection::getUri(void) const
{
	return _uri;
}

void Connection::setTarget(const string &target)
{
	_target = target;
}

string Connection::getTarget(void) const
{
	return _target;
}

void Connection::setProtocol(const string &protocol)
{
	_protocol = protocol;
}

string Connection::getProtocol(void) const
{
	return _protocol;
}

void Connection::setCode(const string &code)
{
	_code = code;
}

string Connection::getCode(void) const
{
	return _code;
}

void Connection::setStatus(const string &status)
{
	_status = status;
}

string Connection::getStatus(void) const
{
	return _status;
}

void Connection::addHeader(const string &key, const string &value)
{
	_headers[key] = value;
}

void Connection::addHeader(const string &key, const size_t &value)
{
	_headers[key] = parser::toString(value);
}

void Connection::setHeaders(const map<string, string> &headers)
{
	_headers = headers;
}

string Connection::getHeaderByKey(const string &key) const
{
	map<string, string>::const_iterator header = _headers.find(key);
	if (header->first == key)
		return header->second;

	return "";
}

map<string, string> Connection::getHeaders(void) const
{
	return _headers;
}

size_t Connection::getHeadersSize(void) const
{
	ostringstream oss;

	map<string, string>::const_iterator header = _headers.begin();
	for (; header != _headers.end(); header++)
		oss << header->first << ": " << header->second << endl;

	return oss.str().size();
}

void Connection::addBody(const std::string &body)
{
	_body.append(body);
}

void Connection::setBody(const string &body)
{
	_body = body;
}

string Connection::getBody(void) const
{
	return _body;
}

void Connection::setResource(Resource *file)
{
	_garbage.push_back(file);
	_file = file;
}

void Connection::setServer(const Server &server)
{
	_server = server;
}

Server &Connection::getServer(void)
{
	return _server;
}

void Connection::setLocation(const Location &location)
{
	_location = location;
}

Location &Connection::getLocation(void)
{
	return _location;
}

void Connection::buildResponse(void)
{
	if (_file && _file->getStep() < step::CLOSE)
		return;

	response::printLog(this);

	if (getHeaderByKey(header::CONNECTION) != "keep-alive")
		_headers[header::CONNECTION] = "close";
	else
		_transfers++;

	if (_file)
	{
		_headers[header::CONTENT_LENGTH] = parser::toString(_file->getSize());
		if (!dynamic_cast<Cgi *>(_file))
			_headers[header::CONTENT_TYPE] = _file->getMime();
	}
	_headers[header::SERVER] = standard::SERVER_SOFTWARE;

	ostringstream oss;
	oss <<  _protocol + " " + _code + " " + _status + "\r\n";

	map<string, string>::iterator header = _headers.begin();
	for (; header != _headers.end(); header++)
		oss << header->first + ": " + header->second + "\r\n";

	_output = oss.str() + "\r\n";
	_step = step::RESPONSE;
	WebServ::getInstance()->controlEpoll(_fd, EPOLLOUT | EPOLLET, EPOLL_CTL_MOD);
}

void Connection::processOutput(const size_t &bytes)
{
	if (_file)
		_output += _file->getData(bytes);

	if (_output.empty())
	{
		if ((*this)[header::CONNECTION] != "keep-alive")
			_step = step::CLOSE;
		else
			_step = step::KEEPALIVE;
	}
}

void Connection::resetConnection(void)
{
	_input.clear();
	_output.clear();
	_size = 0;
	_step = step::NONE;

	_host.clear();
	_method.clear();
	_target.clear();
	_protocol.clear();
	_code.clear();
	_status.clear();
	_headers.clear();
	_body.clear();

	if (_uri)
		delete _uri;

	_uri = NULL;
	_file = NULL;

	_time = time(NULL);
}

bool Connection::isKeepAliveTimedOut(void) const
{
	if (_transfers && time(NULL) - _time > standard::KEEP_ALIVE_TIMEOUT)
		return true;

	return false;
}

void Connection::sendTimeOut(void)
{
	if (_file && dynamic_cast<Cgi *>(_file))
		WebServ::getInstance()->controlEpoll(_file->getFd(), 0, EPOLL_CTL_DEL);

	response::builder(this, code::GATEWAY_TIMEOUT);
}

void Connection::setTime(void)
{
	struct tm tm_info;
	char buffer[128];
	time_t now = time(NULL);

	gmtime_r(&now, &tm_info);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &tm_info);
	addHeader(header::DATE, buffer);
}

std::string Connection::operator[](const std::string &key)
{
	static string empty;

	map<string, string>::iterator header = _headers.find(key);
	if (header->first == key)
		return header->second;

	return empty ;
}

bool Connection::operator==(const string &key)
{
	if (_headers.find(key) == _headers.end())
		return false;

	return true;
}

ostream &operator<<(ostream &os, const Connection &src) {

	os << "Connection" << endl;
	os << "client_fd: " << src.getFd() << endl;
	//os << "IP: " << src.getIp() << endl;
	os << "hostname: " << src.getHost() << endl;
	//os << "buffer: " << src.getBuffer() << endl;
	os << "method: " << src.getMethod() << endl;
	os << "target: " << src.getTarget() << endl;
	os << "protocol: " << src.getProtocol() << endl;
	os << "code: " << src.getCode() << endl;
	os << "status: " << src.getStatus() << endl;
	//os << "request headers: " << src.getHeaders() << endl;
	os << "request body: " << src.getBody() << endl;
	//os << "http {\n" << src.getServer() << "\n}" << endl;
	//os << "response: " << src.getResponse() << endl;

	return os;
}
