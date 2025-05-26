#ifndef CODE_HPP
#define CODE_HPP

#include <string>

namespace code
{
	static const std::string OK = "200";
	static const std::string CREATED = "201";
	static const std::string ACCEPTED = "202";
	static const std::string NO_CONTENT = "204";
	static const std::string MOVED_PERMANENTLY = "301";
	static const std::string FOUND = "302";
	static const std::string BAD_REQUEST = "400";
	static const std::string UNAUTHORIZED = "401";
	static const std::string FORBIDDEN = "403";
	static const std::string NOT_FOUND = "404";
	static const std::string NOT_ALLOWED = "405";
	static const std::string CONFLICT = "409";
	static const std::string LENGTH_REQUIRED = "411";
	static const std::string PAYLOAD_TOO_LARGE = "413";
	static const std::string URI_TOO_LONG = "414";
	static const std::string UNSUPPORTED_MEDIA_TYPE = "415";
	static const std::string I_AM_A_TEAPOT = "418";
	static const std::string UNPROCESSABLE_CONTENT = "422";
	static const std::string INTERNAL_SERVER_ERROR = "500";
	static const std::string NOT_IMPLEMENTED = "501";
	static const std::string BAD_GATEWAY= "502";
	static const std::string SERVICE_UNAVAILABLE = "503";
	static const std::string GATEWAY_TIMEOUT = "504";
	static const std::string HTTP_VERSION_NOT_SUPPORTED = "505";
	static const std::string INSUFFICIENT_STORAGE = "507";
}

#endif // CODE_HPP
