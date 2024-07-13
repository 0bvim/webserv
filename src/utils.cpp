#include "common.hpp"

std::string itoa(int value)
{
	std::string str;
	std::stringstream ss;
	ss << value;
	ss >> str;
	return str;
}
