#ifndef ENV_HPP
#define ENV_HPP

#include <string>

namespace env
{
	static const std::string AUTHORIZATION = "AUTHORIZATION";
	static const std::string CONTENT_LENGTH = "CONTENT_LENGTH";
	static const std::string CONTENT_TYPE = "CONTENT_TYPE";
	static const std::string GATEWAY_INTERFACE = "GATEWAY_INTERFACE";
	static const std::string HTTP_PREFIX = "HTTP_";
	static const std::string PATH_INFO = "PATH_INFO";
	static const std::string PATH_TRANSLATED= "PATH_TRANSLATED";
	static const std::string QUERY_STRING = "QUERY_STRING";
	static const std::string REDIRECT_STATUS = "REDIRECT_STATUS";
	static const std::string REMOTE_ADDR = "REMOTE_ADDR";
	static const std::string REMOTE_HOST = "REMOTE_HOST";
	static const std::string REMOTE_PORT = "REMOTE_PORT";
	static const std::string REQUEST_METHOD = "REQUEST_METHOD";
	static const std::string SCRIPT_NAME = "SCRIPT_NAME";
	static const std::string SCRIPT_FILENAME = "SCRIPT_FILENAME";
	static const std::string SERVER_NAME = "SERVER_NAME";
	static const std::string SERVER_PROTOCOL= "SERVER_PROTOCOL";
	static const std::string SERVER_SOFTWARE = "SERVER_SOFTWARE";
}

#endif // ENV
