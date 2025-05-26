#include "directive.hpp"
#include "Http.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "logger.hpp"
#include "method.hpp"
#include "parser.hpp"
#include "size.hpp"
#include "standard.hpp"
#include <limits>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

bool directive::validateHttpListen(string listen)
{
	if (listen.empty())
		return false;

	if (listen.find_first_not_of(".:0123456789") != string::npos)
		return false;

	if (listen.at(0) == ':' || parser::lastCharacter(listen) == ':')
		return false;

	if (listen.find("::") != string::npos)
		return false;

	list<string> tmp = parser::split(listen, ':');
	if (tmp.size() > 2)
		return false;

	return true;
}

bool directive::validateHttpHost(string host)
{
	if (host.empty())
		return false;

	if (host.find_first_not_of(".0123456789") != string::npos)
		return false;

	if (host.find("..") != string::npos)
		return false;

	if (host.at(0) == '.' || parser::lastCharacter(host) == '.')
		return false;

	list<string> octets = parser::split(host, '.');

	if (octets.size() != 4)
		return false;

	list<string>::iterator it = octets.begin();
	for (; it != octets.end(); ++it)
		if (parser::toSizeT(*it) > 255)
			return false;

	return true;
}

bool directive::validateHttpPort(string port)
{
	if (port.empty())
		return false;

	if (port.find_first_not_of("0123456789") != string::npos)
		return false;

	size_t port_value = parser::toSizeT(port);

	if (port_value < 1024 || port_value > 49151)
		return false;

	return true;
}

void directive::addListen(string listen, vector<string> &_listen)
{
	if (listen.empty())
		return;

	string host = standard::HOST;
	string port = standard::PORT;

	if (not directive::validateHttpListen(listen))
		throw runtime_error("invalid listen: " + listen);

	list<string> tmp = parser::split(listen, ':');

	if (tmp.size() >= 1 && directive::validateHttpPort(tmp.back()))
		port = tmp.back();
	else if (tmp.size() == 1 && directive::validateHttpHost(tmp.back()))
		host = tmp.back();
	else
		throw runtime_error("invalid listen: " + listen);

	if (tmp.size() == 2 && directive::validateHttpHost(tmp.front()))
		host = tmp.front();
	else if (tmp.size() == 2)
		throw runtime_error("invalid host: " + tmp.front());

	listen = host + ":" + port;

	vector<string>::iterator it = _listen.begin();
	for (; it != _listen.end(); ++it)
		if (*it == listen)
			throw runtime_error("duplicated listen: " + listen);

	_listen.push_back(listen);
}

bool directive::validateName(string name)
{
	if (name.empty())
		return false;

	if (name.at(0) == '-' || parser::lastCharacter(name) == '-')
		return false;

	if (name.at(0) == '.' || parser::lastCharacter(name) == '.')
		return false;

	for (string::iterator it = name.begin(); it != name.end(); it++)
	{
		if (!isalnum(*it) && *it != '-' && *it != '.')
			return false;

		if (*it == '-' && *(it + 1) == '-')
			return false;

		if (*it == '.' && *(it + 1) == '.')
			return false;
	}

	return true;
}

void directive::addName(string name, vector<string> &_name)
{
	if (name.empty())
		return;

	list<string> names = parser::split(name, ' ');

	for (list<string>::iterator it = names.begin(); it != names.end(); it++) {
		if (!directive::validateName(*it))
			throw runtime_error("invalid server name: " + *it);

		_name.push_back(parser::toLower(*it));
	}
}

bool directive::isValidRequestTarget(const string& target)
{
	return isValidAbsolutePath(target) || isValidAbsoluteURI(target);
}

bool directive::isValidAbsolutePath(const string& target)
{
	if (target.empty() || target[0] != '/')
		return false;

	if (target.find_first_not_of(standard::ALLOWED_CHARACTERS) != string::npos)
		return false;

	return true;
}

bool directive::isValidAbsoluteURI(const string& target)
{
	size_t schemeEnd = target.find("://");
	if (schemeEnd == string::npos)
		return false;

	for (size_t i = 0; i < schemeEnd; ++i)
	{
		char c = target[i];
		if (!isalpha(c))
			return false;
	}

	size_t pathStart = schemeEnd + 3;
	if (pathStart >= target.size())
		return false;

	return isValidAbsolutePath(target.substr(pathStart));
}

void directive::setURI(string uri, string &_uri)
{
	if (uri.empty())
		return;

	if (!directive::isValidAbsolutePath(uri))
		throw runtime_error("invalid path: " + uri);

	_uri = parser::formatPath(uri);
}

bool directive::validateHttpMethod(string method)
{
	set<string> &allowed_methods = method::getAllowedMethods();

	set<string>::iterator it = allowed_methods.begin();
	for (; it != allowed_methods.end(); it++)
		if (*it == method)
			return true;

	return false;
}

void directive::addMethod(string method, set<string> &_allow_methods)
{
	if (method.empty())
		return;

	list<string> methods = parser::split(method, ' ');

	_allow_methods.clear();

	list<string>::iterator it = methods.begin();
	for (; it != methods.end(); it++)
	{
		if (not validateHttpMethod(*it))
			throw runtime_error("invalid method: " + *it);

		_allow_methods.insert(*it);
	}
}

void directive::setDenyMethods(string deny_methods, bool &_deny_methods)
{
	if (deny_methods.empty())
		return;

	if (deny_methods == "all")
		_deny_methods = true;
	else
		throw runtime_error("invalid deny: " + deny_methods);
}

void directive::setRoot(string root, string &_root)
{
	if (root.empty())
		return;

	if (root.find_first_of(" ") != string::npos)
		throw runtime_error("invalid root: " + root);

	_root = root;
}

void directive::setAutoIndex(string autoindex, bitset<2> &_autoindex)
{
	if (autoindex.empty())
		return;

	if (autoindex == "on")
		_autoindex = parser::AUTOINDEX_ON;
	else if (autoindex == "off")
		_autoindex = parser::AUTOINDEX_OFF;
	else
		throw runtime_error("invalid autoindex: " + autoindex);
}

void directive::setWebDav(string webdav, bitset<2> &_webdav)
{
	if (webdav.empty())
		return;

	if (webdav== "on")
		_webdav= parser::WEB_DAV_ON;
	else if (webdav == "off")
		_webdav= parser::WEB_DAV_OFF;
	else
		throw runtime_error("invalid webdav: " + webdav);
}

void directive::setMaxBodySize(string max_body_size, size_t &_max_body_size)
{
	if (max_body_size.empty())
		return;

	size_t pos = max_body_size.find_first_not_of("0123456789");

	string format;
	if (pos != string::npos)
		format = max_body_size.substr(pos, max_body_size.size() - pos);

	size_t tmp = parser::toSizeT(max_body_size);
	if (tmp == 0)
		_max_body_size = numeric_limits<size_t>::max();
	else
		_max_body_size = tmp;

	if (format.empty() || format == "B")
		_max_body_size *= size::BYTE;
	else if (format == "K")
		_max_body_size *= size::KILOBYTE;
	else if (format == "M")
		_max_body_size *= size::MEGABYTE;
	else if (format == "G")
		_max_body_size *= size::GIGABYTE;
	else
		throw runtime_error("invalid value to max_body_size: " + max_body_size);
}

void directive::addIndex(string index, set<string> &_index)
{
	if (index.empty())
		return;

	list<string> indexes = parser::split(index, ' ');

	_index.clear();

	list<string>::iterator it = indexes.begin();
	for (; it != indexes.end(); it++)
		_index.insert(*it);
}

void directive::setFastCgi(string fastcgi, string &_fastcgi)
{
	if (fastcgi.empty())
		return;

	_fastcgi = fastcgi;
}

void directive::setFastCgiExtension(string extensions, set<string> &_extensions)
{
	if (extensions.empty())
		return;

	_extensions.clear();

	list<string> tmp = parser::split(extensions, ' ');

	for (list<string>::iterator it = tmp.begin(); it != tmp.end(); it++)
		_extensions.insert((it->at(0) == '.' ? *it : "." + *it));
}

void directive::addErrorPage(string error_page, map<string, string> &_error_pages)
{
	if (error_page.empty())
		return;

	list<string> tmp = parser::split(error_page, ' ');

	if (tmp.size() < 2)
		throw runtime_error("invalid error page: " + error_page);

	if (tmp.back().at(0) != '.' && tmp.back().at(0) != '/')
		tmp.back() = "./" + tmp.back();

	string path = tmp.back();
	tmp.pop_back();

	for (list<string>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		if (not directive::validateHttpCode(*it))
			throw runtime_error("invalid error code: " + *it);

		_error_pages[*it] = path;
	}
}

void directive::mergeErrorPages(map<string, string> error_pages, map<string, string> &_error_pages)
{
	map<string, string>::iterator it = error_pages.begin();
	for (; it != error_pages.end(); it++)
		if (_error_pages[it->first].empty())
			_error_pages[it->first] = it->second;
}

bool directive::validateHttpCode(string code)
{
	if (code.find_first_not_of("0123456789") != string::npos)
		return false;

	size_t tmp = parser::toSizeT(code);
	if (tmp < 200 || tmp > 599)
		return false;

	return true;
}

static bool validateReturn(string &tmp)
{
	if (tmp.empty())
		return true;

	string quote = string(1, tmp.at(0));
	if (quote.at(0) != '\'' && quote.at(0) != '\"')
		return true;

	size_t first = tmp.find_first_of(quote);
	size_t last = tmp.find_last_of(quote);
	if (first == string::npos && last == string::npos)
		return true;

	if (first == string::npos || last == string::npos)
		return false;

	first++;

	string text = tmp.substr(first, last - first);
	if (text.size() != tmp.size() - 2)
		return false;

	if (text.find_first_of(quote) != string::npos)
		return false;

	tmp = text;

	return true;
}

void directive::setReturn(string value, string &_code, string &_uri)
{
	if (value.empty())
		return;

	list<string> tmp = parser::split(value, ' ');

	if (tmp.size() < 1 || tmp.size() > 2)
		throw runtime_error("invalid return: " + value);

	if (not directive::validateHttpCode(tmp.front()))
		throw runtime_error("invalid return code: " + tmp.front());

	_code = tmp.front();

	if (tmp.size() == 1)
		return _uri.clear();

	if (!validateReturn(tmp.back()))
		throw runtime_error("invalid return uri/text: " + tmp.back());

	_uri = tmp.back();
}

void directive::addServer(Server server, vector<Server> &_servers)
{
	vector<Server>::iterator serverIt = _servers.begin();
	for (; serverIt != _servers.end(); serverIt++)
	{
		vector<string> exitenListen = serverIt->getListen();
		vector<string>::iterator exitenListenIt = exitenListen.begin();
		for (; exitenListenIt != exitenListen.end(); exitenListenIt++)
		{
			list<string> tmp = parser::split(*exitenListenIt, ':');

			vector<string> newListen = server.getListen();
			vector<string>::iterator newListenIt = newListen.begin();
			for (; newListenIt != newListen.end(); newListenIt++)
			{
				list<string> tmp2 = parser::split(*newListenIt, ':');
				if (tmp2.front() != standard::HOST
					&& tmp.front() == tmp2.front()
					&& tmp.back() == tmp2.back())
				{
					logger::warning("conflicting server name \""
						+ (server.getNames().size() ? server.getNames().at(0) : "")
						+ "\" on " + *newListenIt + ", ignored");

					newListen.erase(newListenIt);
					server.setListen(newListen);

					return directive::addServer(server, _servers);
				}
			}
		}
	}

	_servers.push_back(server);
}

void directive::setHttpDefaultValues(Http &http)
{
	if (http.getMaxBodySize() == 0)
		http.setMaxBodySize(standard::MAX_BODY_SIZE);

	if (http.getIndexes().size() == 0)
		http.addIndex(standard::DEFAULT_INDEXES);

	if (http.getAutoIndexBitSet() == parser::AUTOINDEX_NOT_SET)
		http.setAutoIndex(parser::AUTOINDEX_OFF);

	if (http.getWebDavBitSet() == parser::WEB_DAV_NOT_SET)
		http.setWebDav(parser::WEB_DAV_OFF);

	if (http.getRoot().empty())
		http.setRoot(standard::ROOT_DIR);

	vector<Server> servers = http.getServers();
	vector<Server>::iterator it = servers.begin();
	for (; it != servers.end(); it++)
		directive::setServerDefaultValues(http, *it);

	http.setServers(servers);
}

void directive::setServerDefaultValues(Http &http, Server &server)
{
	if (server.getMaxBodySize() == 0)
		server.setMaxBodySize(parser::toString(http.getMaxBodySize()));

	if (server.getRoot().empty())
		server.setRoot(http.getRoot());

	if (server.getIndexes().empty())
		server.setIndexes(http.getIndexes());

	if (server.getAutoIndexBitSet() == parser::AUTOINDEX_NOT_SET)
		server.setAutoIndex(http.getAutoIndexBitSet());

	if (server.getWebDavBitSet() == parser::WEB_DAV_NOT_SET)
		server.setWebDav(http.getWebDavBitSet());

	map<string, string> error_pages = server.getErrorPages();
	directive::mergeErrorPages(http.getErrorPages(), error_pages);
	server.setErrorPages(error_pages);

	map<string, Location> locations = server.getLocations();
	map<string, Location>::iterator it = locations.begin();
	for (; it != locations.end(); it++)
		directive::setLocationDefaultValues(server, it->second);

	server.setLocations(locations);
}

void directive::setLocationDefaultValues(Server &server, Location &location)
{
	if (location.getRoot().empty())
		location.setRoot(server.getRoot());

	if (location.getIndexes().empty())
		location.setIndexes(server.getIndexes());

	if (location.getDenyMethods() == false)
		location.setMethods(method::getAllowedMethods());

	if (location.getAutoIndexBitSet() == parser::AUTOINDEX_NOT_SET)
		location.setAutoIndex(server.getAutoIndexBitSet());

	if (location.getWebDavBitSet() == parser::WEB_DAV_NOT_SET)
		location.setWebDav(server.getWebDavBitSet());

	if (location.getMaxBodySize() == 0)
		location.setMaxBodySize(parser::toString(server.getMaxBodySize()));

	map<string, string> error_pages = location.getErrorPages();
	directive::mergeErrorPages(server.getErrorPages(), error_pages);
	location.setErrorPages(error_pages);

	if (server.getReturnCode().size())
		location.setReturn(server.getReturnCode() + " " + server.getReturnURI());
}
