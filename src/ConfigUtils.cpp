#include "Config.hpp"

const std::string Config::getServerAddress() const
{
	std::string str = this->servers.at(0).listen;
	return str.substr(0, str.find(":"));
}

int Config::getServerPort() const
{
	std::string str = this->servers.at(0).listen;
	return atoi(str.substr(str.find(":") + 1).c_str());
}
