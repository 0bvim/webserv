#include "Connection.hpp"
#include "File.hpp"
#include "Page.hpp"
#include "Text.hpp"
#include "code.hpp"
#include "header.hpp"
#include "logger.hpp"
#include "process.hpp"
#include "response.hpp"
#include "standard.hpp"
#include "status.hpp"
#include "step.hpp"
#include <map>
#include <string>

using namespace std;

void response::printLog(Connection *connection)
{
	const string &message = connection->getIp() + " ["
		+ connection->getCode() + "]: "
		+ connection->getMethod() + " "
		+ connection->getTarget() + " - "
		+ connection->getStatus();

	if (connection->getCode() < code::BAD_REQUEST)
		logger::info(message);
	else
		logger::warning(message);
}

static void buildHeaderAndBody(Connection *connection)
{
	connection->setStep(step::BODY);

	const string &header_connection = (*connection)[header::CONNECTION];
	const string &header_location = (*connection)[header::LOCATION];

	connection->setProtocol(standard::PROTOCOL);
	connection->setHeaders(standard::EMPTY_HEADER);

	connection->addHeader(header::CONNECTION, header_connection);
	connection->addHeader(header::LOCATION, header_location);
	connection->setTime();

	connection->buildResponse();
}

static bool checkErrorPages(Connection *connection)
{
	const string &code = connection->getCode();
	const string &page = connection->getLocation().getErrorPageByCode(code);
	if (page.empty())
		return false;

	const string &path = connection->getTarget();

	connection->setTarget(page);
	URL *uri = new URL(connection);

	connection->setTarget(path);
	if (!uri->isFile())
	{
		delete uri;
		return false;
	}
	
	URL *old_uri = connection->getUri();
	connection->setUri(uri);

	connection->setResource(new File(connection));
	connection->setUri(old_uri);

	delete uri;

	return true;
}

static bool checkReturn(Connection *connection)
{
	const Location &location = connection->getLocation();
	string return_code = location.getReturnCode();
	string return_uri = location.getReturnURI();

	if (return_code.empty())
		return false;

	if (return_uri.size())
		connection->addHeader(header::LOCATION, location.getReturnURI());

	string status = response::getStatusByCode(return_code);
	if (status.empty())
	{
		return_code = code::INTERNAL_SERVER_ERROR;
		status = status::INTERNAL_SERVER_ERROR;
		return_uri.clear();	
	}

	connection->setCode(return_code);
	connection->setStatus(status);

	if (return_code.at(0) == '3')
		connection->setResource(new Page(connection));
	else if (return_uri.empty() && return_code.at(0) != '2')
		connection->setResource(new Page(connection));
	else
		connection->setResource(new Text(connection));

	return true;
}

string response::getStatusByCode(const string &code)
{
	if (responses.empty())
	{
		responses[code::OK] = status::OK;
		responses[code::CREATED] = status::CREATED;
		responses[code::ACCEPTED] = status::ACCEPTED;
		responses[code::NO_CONTENT] = status::NO_CONTENT;
		responses[code::MOVED_PERMANENTLY] = status::MOVED_PERMANENTLY;
		responses[code::BAD_REQUEST] = status::BAD_REQUEST;
		responses[code::UNAUTHORIZED] = status::UNAUTHORIZED;
		responses[code::FORBIDDEN] = status::FORBIDDEN;
		responses[code::NOT_FOUND] = status::NOT_FOUND;
		responses[code::NOT_ALLOWED] = status::NOT_ALLOWED;
		responses[code::CONFLICT] = status::CONFLICT;
		responses[code::LENGTH_REQUIRED] = status::LENGTH_REQUIRED;
		responses[code::PAYLOAD_TOO_LARGE] = status::PAYLOAD_TOO_LARGE;
		responses[code::URI_TOO_LONG] = status::URI_TOO_LONG;
		responses[code::UNSUPPORTED_MEDIA_TYPE] = status::UNSUPPORTED_MEDIA_TYPE;
		responses[code::I_AM_A_TEAPOT] = status::I_AM_A_TEAPOT;
		responses[code::UNPROCESSABLE_CONTENT] = status::UNPROCESSABLE_CONTENT;
		responses[code::INTERNAL_SERVER_ERROR] = status::INTERNAL_SERVER_ERROR;
		responses[code::NOT_IMPLEMENTED] = status::NOT_IMPLEMENTED;
		responses[code::BAD_GATEWAY] = status::BAD_GATEWAY;
		responses[code::SERVICE_UNAVAILABLE] = status::SERVICE_UNAVAILABLE;
		responses[code::GATEWAY_TIMEOUT] = status::GATEWAY_TIMEOUT;
		responses[code::HTTP_VERSION_NOT_SUPPORTED] = status::HTTP_VERSION_NOT_SUPPORTED;
		responses[code::INSUFFICIENT_STORAGE] = status::INSUFFICIENT_STORAGE;
	}

	map<string, string>::iterator response = responses.find(code);
	if (response == responses.end())
		return "";
	
	return response->second;
}

void response::builder(Connection *connection, string code)
{
	string status = getStatusByCode(code);
	if (status.empty())
		return builder(connection, code::INTERNAL_SERVER_ERROR);

	connection->setCode(code);
	connection->setStatus(status);

	if (checkReturn(connection))
		code = connection->getCode();
	else if (code == code::OK)
		process::request(connection);
	else if (!checkErrorPages(connection))
		connection->setResource(new Page(connection));

	if (code == connection->getCode())
		buildHeaderAndBody(connection);
}
