#include "../include/Config.hpp"
#include <stdexcept>

void Config::checkFile() const
{
	struct stat buffer;

	stat(this->filePath.c_str(), &buffer);
	if (buffer.st_mode & S_IFREG && !access(this->filePath.c_str(), R_OK))
		return;
	else
		throw std::runtime_error("Invalid or not accessible file \U0001F972");
}
