#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Connection.hpp"

namespace request
{
	void parseRequest(Connection *connection, std::string &line);
	void parseStartLine(Connection *connection, std::string &line);
	void parseHeaders(Connection *connection, std::string &line);
	void parseBody(Connection *connection, std::string &line);
	void checkTransferEncodingEnd(Connection *connection, std::string &buffer);
	void parseTransferEncoding(Connection *connection, std::string &line);
	void convertToHex(Connection *connection, std::string &line, size_t &chunck_size);
	void validateHeader(Connection *connection, std::string &key, std::string &value);
	void validateContentLength(Connection *connection, std::string &value);
	void validateHost(Connection *connection, std::string &value);
	void validateTransferEncoding(Connection *connection, std::string &value);
}

#endif // REQUEST_HPP
