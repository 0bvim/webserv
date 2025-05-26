#ifndef PARSER_HPP
#define PARSER_HPP

#include <list>
#include <string>

class Http;
class Server;
class Location;

namespace parser
{
	enum {
		AUTOINDEX_NOT_SET = -1,
		AUTOINDEX_OFF = 0,
		AUTOINDEX_ON = 1
	};

	enum {
		WEB_DAV_NOT_SET = -1,
		WEB_DAV_OFF = 0,
		WEB_DAV_ON = 1
	};

	const char &lastCharacter(const std::string &text);
	size_t toSizeT(const std::string &value);
	std::string toString(const std::size_t &value);
	std::string toUpper(std::string text);
	std::string toLower(std::string text);
	std::string find(const std::string &key, std::string &configuration_file, const std::string &delimiter);
	bool compare(const std::string &key, std::string &configuration_file);
	std::list<std::string> split(std::string text, const char &delimiter);
	std::string basename(const std::string &text);
	std::string formatPath(const std::string &path);
	void erase(std::string &buffer, const std::string &text, const size_t &quantity);
	void rerase(std::string &buffer, const std::string &text, const size_t &quantity);
	void replace(std::string &buffer, const char &from, const char &to);
	void trim(std::string &buffer, const std::string &set);
	void http(Http &http, std::string &configuration_file);
	void server(Server &server, std::string &configuration_file);
	void location(Location &location, std::string &configuration_file);
	void limit_except(Location &location, std::string &configuration_file);
}

#endif // PARSER_HPP
