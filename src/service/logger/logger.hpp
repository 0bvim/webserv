#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

#ifndef LOGGER_DEBUG
#define LOGGER_DEBUG true
#endif // LOGGER_DEBUG

namespace logger
{
	void debug(const std::string &message);
	void info(const std::string &message);
	void warning(const std::string &message);
	void error(const std::string &message);
	void fatal(const std::string &message);
}

#endif // LOGGER_HPP
