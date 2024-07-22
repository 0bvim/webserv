#include "Config.hpp"

const std::string	Config::getServerAddress() const 
{

	if (!this->servers.at(0).server_name.compare("localhost;"))
		return std::string("127.0.0.1");
	else
		return this->servers.at(0).server_name;
}

int Config::getServerPort() const 
{
	return atoi(this->servers.at(0).listen.c_str());

}
