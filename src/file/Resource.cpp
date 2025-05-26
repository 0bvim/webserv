#include "Resource.hpp"
#include "Connection.hpp"
#include "Mime.hpp"
#include "WebServ.hpp"
#include <string>

using namespace std;

Resource::Resource(Connection *connection)
	: AStream(-1, connection->getTarget()), _connection(connection) {}

Resource::Resource(const Resource &src) : AStream(src)
{
	*this = src;
}

Resource &Resource::operator=(const Resource &rhs)
{
	if (this == &rhs)
		return *this;

	_type = rhs._type;
	_connection = rhs._connection;

	return *this;
}

Resource::~Resource(void)
{
	WebServ::getInstance()->delStream(_fd);
}

string Resource::getMime(void) const
{
	if (_type.empty())
		return Mime::getInstance()->getType(_id);

	return _type;
}
