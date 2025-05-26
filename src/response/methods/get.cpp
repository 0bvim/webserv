#include "Cgi.hpp"
#include "Connection.hpp"
#include "Directory.hpp"
#include "Location.hpp"
#include "File.hpp"
#include "code.hpp"
#include "header.hpp"
#include "parser.hpp"
#include "process.hpp"
#include "response.hpp"

void process::methodGet(Connection *connection)
{
	const URL *uri = connection->getUri();
	const Location &location = connection->getLocation();

	connection->addHeader(header::LOCATION, uri->getLocation());
	
	if (uri->isCgi())
		return connection->setResource(new Cgi(connection));

	if (uri->isFile())
		return connection->setResource(new File(connection));
	
	if (!uri->isDirectory())
		return response::builder(connection, code::NOT_FOUND);

	if (parser::lastCharacter(uri->getAbsolutePath()) == '/')
	{
		if (location.getAutoIndex())
			return connection->setResource(new Directory(connection));

		return response::builder(connection, code::FORBIDDEN);
	}

	connection->addHeader(header::LOCATION, uri->getLocation() + '/');
	response::builder(connection, code::MOVED_PERMANENTLY);
}
