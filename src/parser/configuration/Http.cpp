#include "Http.hpp"
#include "Mime.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "directive.hpp"
#include <fstream>
#include <ostream>
#include <string>

using namespace std;

Http *Http::_instance = NULL;

Http::Http(void)
	: _autoindex(parser::AUTOINDEX_NOT_SET),
	  _webdav(parser::WEB_DAV_NOT_SET),
	  _max_body_size(0),
	  _signal(0) {}

Http::~Http(void)
{
	delete WebServ::getInstance();
	delete Mime::getInstance();

}

Http *Http::getInstance(void)
{
	if (_instance == NULL)
		_instance = new Http();

	return _instance;
}

void Http::configure(std::string filename)
{
	if (parser::basename(filename) != ".conf")
		throw runtime_error("invalid .conf file format: " + filename);

	ifstream file(filename.c_str());
	if (not file)
		throw runtime_error("failed to open configuration file: " + filename);

	string buffer;

	for (string line; getline(file, line); buffer.append(line)) {
		
		if (line.find("#") != string::npos)
			line = line.substr(0, line.find_first_of("#"));

		parser::trim(line, " \n\t\r\v\f");
	}

	file.close();

	parser::replace(buffer, '\t', ' ');

	parser::erase(buffer, "  ", 1);
	parser::erase(buffer, " {", 1);
	parser::rerase(buffer, "{ ", 1);
	parser::erase(buffer, " }", 1);
	parser::rerase(buffer, "} ", 1);
	parser::erase(buffer, " ;", 1);
	parser::rerase(buffer, "; ", 1);

	parser::http(*this, buffer);

	if (empty())
		throw runtime_error("no server avaliable");

	logger::info("configuration file parsed: " + filename);

	directive::setHttpDefaultValues(*this);
}

void Http::setRoot(string root)
{
	directive::setRoot(root, _root);
}

string Http::getRoot(void) const
{
	return _root;
}

void Http::setAutoIndex(string autoindex)
{
	directive::setAutoIndex(autoindex, _autoindex);
}

void Http::setAutoIndex(bitset<2> autoindex)
{
	_autoindex = autoindex;
}

bitset<2> Http::getAutoIndexBitSet(void) const
{
	return _autoindex;
}

bool Http::getAutoIndex() const
{
	return _autoindex == parser::AUTOINDEX_ON ? true : false;
}

void Http::setWebDav(string webdav)
{
	directive::setWebDav(webdav, _webdav);
}

void Http::setWebDav(bitset<2> webdav)
{
	_webdav = webdav;
}

bitset<2> Http::getWebDavBitSet(void) const
{
	return _webdav;
}

bool Http::getWebDav() const
{
	return _webdav == parser::WEB_DAV_ON ? true : false;
}

void Http::setMaxBodySize(string max_body_size)
{
	directive::setMaxBodySize(max_body_size, _max_body_size);
}

size_t Http::getMaxBodySize(void) const
{
	return _max_body_size;
}

void Http::addIndex(string index)
{
	directive::addIndex(index, _indexes);

}

void Http::setIndex(set<string> indexes)
{
	_indexes = indexes;
}

set<string> Http::getIndexes(void) const
{
	return _indexes;
}

void Http::addErrorPage(string error_page)
{
	directive::addErrorPage(error_page, _error_pages);
}

void Http::setErrorPages(map<string, string> error_pages)
{
	_error_pages = error_pages;
}

map<string, string> Http::getErrorPages(void) const
{
	return _error_pages;
}

string Http::getErrorPageByCode(string code) const
{
	map<string, string>::const_iterator error_page = _error_pages.find(code);
	if (error_page ==_error_pages.end())
		return "";

	return error_page->second;
}

void Http::addServer(Server server)
{
	directive::addServer(server, _servers);
}

void Http::setServers(vector<Server> servers)
{
	_servers = servers;
}

Server Http::getServerByListen(string listen) const
{
	list<string> host = parser::split(listen, ':');
	if (!directive::validateHttpListen(listen) || host.size() != 2)
		return Server();

	vector<Server>::const_iterator server = _servers.begin();
	for (; server != _servers.end(); server++)
	{
		vector<string> listens = server->getListen();
		vector<string>::iterator listen = listens.begin();
		for(; listen != listens.end(); listen++)
		{
			list<string> server_host = parser::split(*listen, ':');
			if (server_host.back() != host.back())
				continue;
			
			if (server_host.front() == "0.0.0.0")
				return *server;

			if (server_host.front() == host.front())
				return *server;
		}
	}

	return Server();
}

Server Http::getServerByName(string name) const
{
	list<string> host = parser::split(name, ':');
	vector<Server>::const_iterator server  = _servers.begin();
	for (; server != _servers.end(); server++)
	{
		set<string> ports;
		vector<string> listens = server->getListen();
		vector<string>::iterator listen = listens.begin();
		for (; listen != listens.end(); listen++) {

			list<string> tmp = parser::split(*listen, ':');
			ports.insert(tmp.back());
		}

		vector<string> names = server->getNames();
		vector<string>::iterator name= names.begin();
		for(; name!= names.end(); name++)
		{
			if (host.front() != *name)
				continue;

			if (ports.find(host.back()) == ports.end())
				continue;

			return *server;
		}
	}

	return Server();
}

vector<Server> Http::getServers(void) const
{
	return _servers;
}

bool Http::empty(void) const
{
	return _servers.empty() || _servers[0].empty();
}

void Http::start(void)
{
	WebServ::getInstance()->run();
}

void Http::stop(int signal)
{
	_signal = signal;

	WebServ::getInstance()->stop();
}

int Http::getSignal(void)
{
	return _signal;
}

ostream &operator<<(ostream &os, const Http &src)
{
	os << "http {" << endl;
	os << "\tclient_max_body_size " << src.getMaxBodySize() << ";" << endl;
	os << "\troot " << src.getRoot() << ";" << endl;
	os << "\tautoindex " << (src.getAutoIndex() ? "on" : "off") << ";" << endl;
	os << "\twebdav " << (src.getWebDav() ? "on" : "off") << ";" << endl;
	
	os << "\tindex";
	set<string> indexs = src.getIndexes();
	for (set<string>::iterator it = indexs.begin(); it != indexs.end(); it++)
		os << " " << *it;
	os << ";" << endl;

	map<string, string> error_pages = src.getErrorPages();
	for (map<string, string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
		os << "\terror_page " << it->first << " " << (*it).second << ";" << endl;

	vector<Server> servers = src.getServers();
	for (vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
		os << *it << endl;

	os << "}";

	return os;
}
