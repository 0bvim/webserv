#include "common.hpp"

std::string itoa(int value)
{
	std::string str;
	std::stringstream ss;
	ss << value;
	ss >> str;
	return str;
}

std::string trim(const std::string &str)
{
	size_t first = 0;
	size_t last = str.size() - 1;

	while (first <= last && std::isspace(str[first]))
		++first;
	while (last >= first && std::isspace(str[last]))
		--last;
	if (first > last)
		return "";
	return str.substr(first, last - first + 1);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}