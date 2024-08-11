#include "../include/Config.hpp"
#include <stdexcept>

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

void Config::checkFile() const
{
  struct stat buffer;

  stat(this->filePath.c_str(), &buffer);
  if (buffer.st_mode & S_IFREG && !access(this->filePath.c_str(), R_OK))
    return ;
  else
    throw std::runtime_error("Invalid or not accessible file \U0001F972");
}
