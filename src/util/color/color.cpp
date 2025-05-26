#include "color.hpp"
#include <string>

using namespace std;

string color::red(void)
{
	return "\001\033[0;91m";
}

string color::red(const string &message)
{
	return red() + message + color::reset();
}

string color::green(void)
{
	return "\001\033[0;92m";
}

string color::green(const string &message)
{
	return green() + message + color::reset();
}

string color::yellow(void)
{
	return "\001\033[0;93m";
}

string color::yellow(const string &message)
{
	return yellow() + message + color::reset();
}

string color::blue(void)
{
	return "\001\033[0;94m";
}

string color::blue(const string &message)
{
	return blue() + message + color::reset();
}

string color::magenta(void)
{
	return "\001\033[0;95m";
}

string color::magenta(const string &message)
{
	return magenta() + message + color::reset();
}

string color::reset(void)
{
	return "\001\033[0;m";
}
