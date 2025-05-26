#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "AStream.hpp"
#include <string>

class Connection;

class Resource : public AStream
{
	protected:
		std::string _type;
		Connection *_connection;

		Resource(Connection *connection);
		Resource(const Resource &src);
		Resource &operator=(const Resource &rhs);

	public:
		virtual ~Resource(void);

		virtual std::string getMime(void) const;
};

#endif // RESOURCE_HPP
