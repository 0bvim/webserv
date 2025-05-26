#include "Connection.hpp"
#include "Cgi.hpp"
#include "URL.hpp"
#include "code.hpp"
#include "header.hpp"
#include "process.hpp"
#include "response.hpp"
#include <fstream>

void process::methodPost(Connection *connection)
{
	URL *uri = connection->getUri();
	connection->addHeader(header::LOCATION, uri->getLocation());

	if (uri->isCgi())
		return connection->setResource(new Cgi(connection));

	if (!connection->getLocation().getWebDav())
		return response::builder(connection, code::FORBIDDEN);

	if (!uri->isDeletable())
		return response::builder(connection, code::FORBIDDEN);

	if (uri->isFile() || uri->isDirectory())
		return response::builder(connection, code::CONFLICT);

	std::ofstream file(uri->getAbsolutePath().c_str());

	if (!file.is_open())
		return response::builder(connection, code::INTERNAL_SERVER_ERROR);

	file << connection->getBody();
	file.close();

	return response::builder(connection, code::CREATED);
}
