#include "logger.hpp"
#include "color.hpp"
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

static const std::string getTime(void)
{
	time_t rawtime = time(&rawtime);
	struct tm *timeinfo = localtime(&rawtime);

	char buffer[22];
	strftime(buffer, sizeof(buffer), "[%d/%m/%Y %H:%M:%S]", timeinfo);

	return buffer;
}

static void print(const string &color, const string &level, const string &message)
{
	cout << color + getTime() + " " + level + ": " + message << color::reset() << endl;
}

void logger::debug(const string &message)
{
	if (not LOGGER_DEBUG)
		return;

	print(color::blue(), "DEBUG", message);
}

void logger::info(const string &message)
{
	print(color::green(), "INFO", message);
}

void logger::warning(const string &message)
{
	print(color::yellow(), "WARNING", message);
}

void logger::error(const string &message)
{
	print(color::red(), "ERROR", message);
}

void logger::fatal(const string &message)
{
	print(color::magenta(), "FATAL", message);
}
