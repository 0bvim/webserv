#ifndef STANDARD_HPP
#define STANDARD_HPP

#include "size.hpp"
#include <map>
#include <string>

namespace standard
{
	static const std::string ROOT_DIR = "./var/www/html";
	static const std::string MAX_BODY_SIZE = "1M";
	static const std::string DEFAULT_INDEXES = "index.htm index.html";
	static const std::string HOST = "0.0.0.0";
	static const std::string PORT = "8080";

	static const std::string GATEWAY_INTERFACE = "CGI/1.1";
	static const std::string PROTOCOL = "HTTP/1.1";
	static const std::string SERVER_SOFTWARE = "webserv/0.1.0";
	static const std::map<std::string, std::string> EMPTY_HEADER;

	static const int BUFFER_SIZE = 128 * size::MEGABYTE;
	static const int MAX_EVENTS = 252;
	static const long KEEP_ALIVE_TIMEOUT = 3;
	static const size_t TIMEOUT = 15;

	static const std::string MIME = "application/octet-stream";

	static const std::string DECIMAL = "0123456789";
	static const std::string HEXADECIMAL = "0123456789ABCDEFabcdef";

	static const std::string ALLOWED_CHARACTERS = ""
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
		"0123456789-._~:/?#[]@!$&'()*+,;=%";
}

#endif // STANDARD_HPP
