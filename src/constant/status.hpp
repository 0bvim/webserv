#ifndef STATUS_HPP
#define STATUS_HPP

#include <string>

namespace status
{
	static const std::string OK = "OK";
	static const std::string CREATED = "Created";
	static const std::string ACCEPTED = "Accepted";
	static const std::string NO_CONTENT = "No Content";
	static const std::string MOVED_PERMANENTLY = "Moved Permanently";
	static const std::string FOUND = "Found";
	static const std::string BAD_REQUEST = "Bad Request";
	static const std::string UNAUTHORIZED = "Unauthorized";
	static const std::string FORBIDDEN = "Forbidden";
	static const std::string NOT_FOUND = "Not Found";
	static const std::string NOT_ALLOWED = "Method Not Allowed";
	static const std::string CONFLICT = "Conflict";
	static const std::string LENGTH_REQUIRED = "Length Required";
	static const std::string PAYLOAD_TOO_LARGE = "Payload Too Large";
	static const std::string URI_TOO_LONG = "URI Too Long";
	static const std::string UNSUPPORTED_MEDIA_TYPE = "Unsupported Media Type";
	static const std::string I_AM_A_TEAPOT = "I'm a teapot";
	static const std::string UNPROCESSABLE_CONTENT = "Unprocessable Content";
	static const std::string INTERNAL_SERVER_ERROR = "Internal Server Error";
	static const std::string NOT_IMPLEMENTED = "Not Implemented";
	static const std::string BAD_GATEWAY = "Bad Gateway";
	static const std::string SERVICE_UNAVAILABLE = "Service Unavailable";
	static const std::string GATEWAY_TIMEOUT = "Gateway Timeout";
	static const std::string HTTP_VERSION_NOT_SUPPORTED = "HTTP Version Not Supported";
	static const std::string INSUFFICIENT_STORAGE = "Insufficient Storage";
}

#endif // STATUS_HPP
