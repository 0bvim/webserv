#include "request.hpp"
#include "Connection.hpp"
#include "code.hpp"
#include "directive.hpp"
#include "header.hpp"
#include "method.hpp"
#include "parser.hpp"
#include "size.hpp"
#include "standard.hpp"
#include "step.hpp"
#include "response.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <string>

using namespace std;

void request::parseRequest(Connection *connection, string &line)
{
	if (connection->getStep() == step::NONE)
		return parseStartLine(connection, line);

	if (connection->getStep() == step::STARTLINE)
		return parseHeaders(connection, line);

	if (connection->getStep() == step::HEADERS)
		return parseBody(connection, line);
}

void request::parseStartLine(Connection *connection, string &line)
{
	string method, target, protocol;

	if (line.find_first_not_of(" \t\v\r") == string::npos)
		return;

	if (line.find_first_not_of(" \t\v") != 0)
		return response::builder(connection, code::BAD_REQUEST);

	istringstream startline(line);
	if (!(startline >> method >> target >> protocol))
		return response::builder(connection, code::BAD_REQUEST);

	if (!directive::validateHttpMethod(method))
		return response::builder(connection, code::NOT_ALLOWED);

	if (!directive::isValidRequestTarget(target))
		return response::builder(connection, code::BAD_REQUEST);

	URL::decode(target);

	if (target.size() > 2 * size::KILOBYTE)
		return response::builder(connection, code::URI_TOO_LONG);

	if (protocol != standard::PROTOCOL)
		return response::builder(connection, code::HTTP_VERSION_NOT_SUPPORTED);

	connection->setMethod(method);
	connection->setTarget(target);
	connection->setProtocol(protocol);
	connection->setStep(step::STARTLINE);
}

void request::parseHeaders(Connection *connection, std::string &line)
{
	if (line == "\r")
	{
		connection->setStep(step::HEADERS);

		connection->setUri(new URL(connection));

		if (!connection->getHeadersSize())
			return response::builder(connection, code::BAD_REQUEST);

		if ((*connection)[header::CONTENT_TYPE].find("multipart/form-data") == 0)
			return;
		
		if (connection->getMethod() == method::POST
			&& !(*connection == header::CONTENT_LENGTH)
			&& !(*connection == header::TRANSFER_ENCONDING))
			return response::builder(connection, code::BAD_REQUEST);

		if (*connection == header::TRANSFER_ENCONDING)
			return;

		if (parser::toSizeT((*connection)[header::CONTENT_LENGTH]) > 0)
			return;
		
		return response::builder(connection, code::OK);
	}

	size_t separator = line.find_first_of(":");
	if (separator == string::npos)
		return response::builder(connection, code::BAD_REQUEST);

	string key = line.substr(0, separator);
	string value = line.substr(separator + 1);

	URL::decode(value);

	if (value.size() > 8 * size::KILOBYTE)
		return response::builder(connection, code::BAD_REQUEST);

	parser::trim(value, " \t\v\r");

	validateHeader(connection, key, value);

	connection->addHeader(key, value);
}

void request::parseBody(Connection *connection, string &line)
{
	if ((*connection)[header::HOST].empty())
		return response::builder(connection, code::BAD_REQUEST);

	if (*connection == header::TRANSFER_ENCONDING)
		return parseTransferEncoding(connection, line);

	size_t body_size = line.size();
	size_t content_length = parser::toSizeT((*connection)[header::CONTENT_LENGTH]);

	if (body_size < content_length)
		return;

	if (body_size > content_length)
		return response::builder(connection, code::PAYLOAD_TOO_LARGE);

	connection->addBody(line);
	line.clear();

	if (connection->getBody().size() > connection->getLocation().getMaxBodySize())
		return response::builder(connection, code::PAYLOAD_TOO_LARGE);

	response::builder(connection, code::OK);
}

void request::checkTransferEncodingEnd(Connection *connection, string &buffer)
{
	if (buffer.size() < 4)
		return;

	if (!parser::compare("0\r\n\r\n", buffer))
		return response::builder(connection, code::BAD_REQUEST);
	buffer.clear();

	connection->setStep(step::BODY);
	return response::builder(connection, code::OK);
}

void request::parseTransferEncoding(Connection *connection, string &buffer)
{
	if (buffer.empty())
		return;

	string chunk_size_value = buffer.substr(0, buffer.find("\r\n"));
	size_t chunk_size_length = chunk_size_value.size() + 2;

	if (chunk_size_value.find_first_not_of(standard::HEXADECIMAL) != string::npos)
		return response::builder(connection, code::BAD_REQUEST);

	if (chunk_size_value == "0")
		return checkTransferEncodingEnd(connection, buffer);

	size_t chunk_line_length;
	convertToHex(connection, chunk_size_value, chunk_line_length);
	if (connection->getCode().size())
		return;

	string chunk_line_value = buffer.substr(chunk_size_length, chunk_line_length);

	if (chunk_line_value.size() != chunk_line_length)
		return;
	buffer.erase(0, chunk_size_length + chunk_line_length);

	connection->addBody(chunk_line_value);
	if (connection->getBody().size() > connection->getLocation().getMaxBodySize())
		return response::builder(connection, code::PAYLOAD_TOO_LARGE);

	if (!parser::compare("\r\n", buffer))
		return response::builder(connection, code::BAD_REQUEST);
	buffer.erase(0, 2);

	return parseTransferEncoding(connection, buffer);
}

void request::convertToHex(Connection *connection, string &line, size_t &chunck_size)
{
	if (line.find_first_not_of(standard::HEXADECIMAL) != string::npos)
		return response::builder(connection, code::BAD_REQUEST);

	char *rest;
	chunck_size = strtoul(line.c_str(), &rest, 16);
	if (rest[0] != '\0')
		return response::builder(connection, code::BAD_REQUEST);
}

void request::validateHeader(Connection *connection, string &key, string &value)
{
	if (key == header::CONTENT_LENGTH)
		return validateContentLength(connection, value);

	if (key == header::HOST)
		return validateHost(connection, value);

	if (key == header::TRANSFER_ENCONDING)
		return validateTransferEncoding(connection, value);
}

void request::validateContentLength(Connection *connection, string &value)
{
	if (connection->getMethod() != method::POST)
		return response::builder(connection, code::BAD_REQUEST);

	if (value.find_first_not_of(standard::DECIMAL) != string::npos)
		return response::builder(connection, code::BAD_REQUEST);

	if (*connection == header::TRANSFER_ENCONDING)
		return response::builder(connection, code::BAD_REQUEST);
}

void request::validateHost(Connection *connection, string &value)
{
	Http *http = Http::getInstance();
	Server server = http->getServerByListen(value);
	if (server.empty())
		server = http->getServerByName(value);

	if (server.empty())
		server = http->getServerByListen(connection->getId());

	if (server.empty())
		return response::builder(connection, code::BAD_REQUEST);

	connection->setHost(value);
	connection->setServer(server);
}

void request::validateTransferEncoding(Connection *connection, string &value)
{
	if (connection->getMethod() != method::POST)
		return response::builder(connection, code::BAD_REQUEST);

	if (value != "chunked")
		return response::builder(connection, code::NOT_IMPLEMENTED);

	if (*connection == header::CONTENT_LENGTH)
		return response::builder(connection, code::BAD_REQUEST);
}
