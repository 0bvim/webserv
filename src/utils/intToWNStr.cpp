#include <sstream>

std::string intToString(int value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

std::wstring intToWstring(int value)
{
	std::wostringstream woss;
	woss << value;
	return woss.str();
}
