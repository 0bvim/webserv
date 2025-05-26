#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include "Resource.hpp"

class Connection;

class Directory : public Resource
{
	public:
		Directory(Connection *connection);
		Directory(const Directory &src);
		Directory &operator=(const Directory &rhs);
		~Directory(void);
};

#endif // DIRECTORY_HPP
