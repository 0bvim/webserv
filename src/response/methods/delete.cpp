#include "Connection.hpp"
#include "Cgi.hpp"
#include "URL.hpp"
#include "code.hpp"
#include "header.hpp"
#include "process.hpp"
#include "response.hpp"
#include <cstdio>

void process::methodDelete(Connection *connection)
{
	URL *uri = connection->getUri();
	connection->addHeader(header::LOCATION, uri->getLocation());

	if (uri->isCgi())
		return connection->setResource(new Cgi(connection));

	if (!connection->getLocation().getWebDav())
		return response::builder(connection, code::FORBIDDEN);

	if (!uri->isFile() && !uri->isDirectory())
		return response::builder(connection, code::NOT_FOUND);

	if (!uri->isDeletable())
		return response::builder(connection, code::FORBIDDEN);

	if (std::remove(uri->getAbsolutePath().c_str()) == -1)
		return response::builder(connection, code::INTERNAL_SERVER_ERROR);

	response::builder(connection, code::NO_CONTENT);
}
