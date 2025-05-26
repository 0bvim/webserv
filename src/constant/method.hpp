#ifndef METHOD_HPP
#define METHOD_HPP

#include <set>
#include <string>

namespace method
{
	static const std::string CONNECT = "CONNECT";
	static const std::string DELETE = "DELETE";
	static const std::string GET = "GET";
	static const std::string HEAD = "HEAD";
	static const std::string OPTIONS = "OPTIONS";
	static const std::string PATCH = "PATCH";
	static const std::string POST = "POST";
	static const std::string PUT = "PUT";
	static const std::string TRACE = "TRACE";

	std::set<std::string> &getAllowedMethods(void);
}

#endif // METHOD_HPP
