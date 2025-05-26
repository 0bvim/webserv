#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <map>
#include <string>

class Connection;
class Location;

namespace response
{
	static std::map<std::string, std::string> responses;

	void printLog(Connection *connection);
	std::string getStatusByCode(const std::string &code);
	void builder(Connection *connection, std::string code);
}

#endif // RESPONSE_HPP
