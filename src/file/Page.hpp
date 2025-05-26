#ifndef PAGE_HPP
#define PAGE_HPP

#include "Resource.hpp"

class Connection;

class Page : public Resource
{
	public:
		Page(Connection *Connection);
		Page(const Page &src);
		Page &operator=(const Page &rhs);
		~Page(void);
};

#endif // PAGE_HPP
