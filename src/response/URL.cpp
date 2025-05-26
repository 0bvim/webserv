#include "URL.hpp"
#include "Connection.hpp"
#include "Location.hpp"
#include "method.hpp"
#include "parser.hpp"
#include "process.hpp"
#include "standard.hpp"
#include <cctype>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

using namespace std;

URL::URL(Connection *connection) : _connection(connection), _dac(0)
{
	string uri = connection->getTarget();

	_scheme = "http";

	list<string> host = parser::split(connection->getHost(), ':');
	if (host.size())
		_host = host.front();

	if (host.size() > 1)
		_port = host.back();

	size_t pos = uri.find_first_of("?");
	if (pos != string::npos)
	{
		_path = uri.substr(0, pos);
		_query = uri.substr(pos + 1);
	}
	else
	{
		_path = uri;
		_query = "";
	}

	pos = _path.find_last_of(".");
	if (pos != string::npos)
	{
		_path_info = _path.substr(pos, _path.size());

		size_t epos = _path_info.find_first_of("/");
		_extension = _path_info.substr(0, epos);
		if (epos != string::npos)
			_path_info = _path_info.substr(epos);
		else
			_path_info.clear();

		_path = _path.substr(0, pos) + _extension;
	}

	pos = _path.find_last_of("/");
	if (pos != string::npos)
		_file = _path.substr(pos + 1, _path.size());

	processPath(_path);
}

URL::URL(const URL &src)
{
	*this = src;
}

URL &URL::operator=(const URL &rhs)
{
	if (this == &rhs)
		return *this;

	_connection = rhs._connection;
	_scheme = rhs._scheme;
	_host = rhs._host;
	_port = rhs._port;
	_path = rhs._path;
	_query = rhs._query;
	_file = rhs._file;
	_extension = rhs._extension;

	return *this;
}

URL::~URL(void) {}

string URL::checkIndex(const Location &location, string &path)
{
	if (parser::lastCharacter(path) != '/')
		return "";

	const set<string> &indexes = location.getIndexes();

	if (_connection->getMethod() == method::DELETE)
		return "";

	set<string>::const_iterator index = indexes.begin();
	for (; index != indexes.end(); ++index)
	{
		if (!_isFile(location.getRoot() + path + *index))
			continue;

		path.append(*index);
		return *index;
	}
	return "";
}

void URL::processPath(string requested_path)
{
	Server server = _connection->getServer();
	_path = parser::formatPath(requested_path);

	list<string> paths;
	while (requested_path.size())
	{
		paths.push_back(requested_path);

		size_t pos = requested_path.find_last_of("/");
		if (pos == 0 || requested_path == ".")
		{
			paths.push_back("/");
			break;
		}

		requested_path = requested_path.substr(0, pos);
	}

	Location location;

	list<string>::iterator path = paths.begin();
	for (; path != paths.end(); path++)
	{
		location = server.getLocationByURI(*path);
		if (!location.empty())
			break;

		if (parser::lastCharacter(_path) != '/')
			continue;

		string tmp = (*path).substr(0, (*path).size() - 1);
		location = server.getLocationByURI(tmp);
		if (!location.empty())
			break;
	}

	if (_connection->getMethod() == method::GET)
		if (_isDirectory(location.getRoot() + _path))
			_file = checkIndex(location, _path);

	size_t pos = _file.find_last_of(".");
	if (pos != string::npos)
		_extension = _file.substr(pos, _file.size());

	_connection->setLocation(location);

	checkDAC(location.getRoot() + _path);
}

bool URL::_isDirectory(const string &path)
{
	struct stat info;

	if (stat(path.c_str(), &info) == 0)
		return S_ISDIR(info.st_mode);

	return false;
}

bool URL::_isFile(const string &path)
{
	struct stat info;

	if (stat(path.c_str(), &info) == 0)
		return S_ISREG(info.st_mode);

	return false;
}

bool URL::_isReadable(const string &path)
{
	if (access(path.c_str(), R_OK) == 0)
		return true;

	return false;
}

bool URL::_isWritable(const string &path)
{
	if (access(path.c_str(), W_OK) == 0)
		return true;

	return false;
}

bool URL::_isExecutable(const string &path)
{
	if (access(path.c_str(), X_OK) == 0)
		return true;

	return false;
}

bool URL::_isDirectoryEmpty(const std::string &path)
{
	DIR* dir = opendir(path.c_str());
	if (!dir)
		return false;

	for (struct dirent *entry = readdir(dir); entry; entry = readdir(dir))
	{
		const string &name = entry->d_name;
		if (name == "." || name == "..")
			continue;

		closedir(dir);
		return false;
	}

	closedir(dir);
	return true;
}

bool URL::_isDeletable(const string &path)
{
	const string &parent_directory = path.substr(0, path.size() - _file.size());

	if (!_isWritable(parent_directory))
		return false;

	if (isDirectory() && _isDirectoryEmpty(path))
		return true;

	if (isFile())
		return true;

	if (_connection->getMethod() == method::POST)
		return true;

	return false;
}

void URL::checkDAC(const string &path)
{
	if (_isFile(path))
		_dac |= FILE;
	else if (_isDirectory(path))
		_dac |= DIRECTORY;

	if (_isReadable(path))
		_dac |= READ;

	if (_isWritable(path))
		_dac |= WRITE;

	if (_isExecutable(path))
		_dac |= EXECUTE;

	if (_connection->getLocation().isCgi(_extension))
		_dac |= CGI;

	if (_isDeletable(path))
		_dac |= DELETE;
}

string URL::getScheme(void) const
{
	return _scheme;
}

string URL::getHost(void) const
{
	return _host;
}

string URL::getPort(void) const
{
	return _port;
}

string URL::getPath(void) const
{
	return _path;
}

string URL::getQuery(void) const
{
	return _query;
}

string URL::getFile(void) const
{
	return _file;
}

string URL::getPathInfo(void) const
{
	return _path_info;
}

string URL::getPathTranslated(void) const
{
	return _connection->getLocation().getRoot() + _path + _path_info;
}

string URL::getExtension(void) const
{
	return _extension;
}

std::string URL::getAbsolutePath(void) const
{
	return _connection->getLocation().getRoot() + _path;
}

std::string URL::getLocation(void) const
{
	return (_scheme.size() ? _scheme : "http") + "://"
		+ _host + (_port.size() ? ":" + _port : "")
		+ _path.substr(0, _path.size() - _file.size());
}

bool URL::isDirectory(void) const
{
	return _dac & DIRECTORY;
}

bool URL::isFile(void) const
{
	return _dac & FILE;
}

bool URL::isCgi(void) const
{
	return _dac & CGI;
}

bool URL::isReadable(void) const
{
	return _dac & READ;
}

bool URL::isWritable(void) const
{
	return _dac & WRITE;
}

bool URL::isExecutable(void) const
{
	return _dac & EXECUTE;
}

bool URL::isDeletable() const
{
	return _dac & DELETE;
}

void URL::decode(std::string &path)
{
    std::string output;

    for (size_t i = 0; i < path.length(); i++)
	{
		if (path.at(i) != '%' || path.length() < i + 2)
		{
			output += path.at(i);
			continue;
		}

		const string &encoding = path.substr(i + 1, 2);

		if (encoding.find_first_not_of(standard::HEXADECIMAL) != string::npos)
		{
			output += '%';
			continue;
		}

		istringstream iss(encoding);
		int value;

		iss >> hex >> value;
		output += static_cast<char>(value);

		i += 2;
	}

    path = output;
}

ostream &operator<<(ostream &os, const URL &src)
{
	os << (src.getScheme().size() ? src.getScheme() + "://" : "http://")
		+ src.getHost() + (src.getPort().size() ? ":" + src.getPort() : "")
		+ src.getPath() + (src.getPathInfo().size() ? src.getPathInfo() : "")
		+ (src.getQuery().size() ? "?" + src.getQuery() : "") << endl;

	os << "file: " << src.getFile() << endl;
	os << "extension: " << src.getExtension() << endl;
	os << "path_info: " << src.getPathInfo() << endl;
	os << "query string: " << src.getQuery() << endl;
	os << "path_translated: " << src.getPathTranslated() << std::endl;
	os << "location: " << src.getLocation() << endl;

	os << "absolute path: " << src.getAbsolutePath() << endl;

	os << "dac: " << (src.isDirectory() ? "d" : "") << (src.isFile() ? "." : "");
	os << (src.isReadable() ? "r" : "-") << (src.isWritable() ? "w" : "-");
	os << (src.isExecutable() ? "x" : "-") << (src.isCgi() ? " cgi" : "");
	os << (src.isDeletable() ? " deletable" : "");

	return os;
}
