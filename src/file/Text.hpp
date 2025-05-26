#ifndef TEXT_HPP
#define TEXT_HPP

#include "Resource.hpp"

class Text : public Resource
{
	public:
		Text(Connection *connection);
		Text(const Text &src);
		Text &operator=(const Text &rhs);
		~Text(void);
};

#endif // TEXT_HPP
