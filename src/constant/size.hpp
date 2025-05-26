#ifndef SIZE_HPP
#define SIZE_HPP

#include <cstddef>

namespace size
{
	static const size_t BYTE = 1;
	static const size_t KILOBYTE = (1024 * BYTE);
	static const size_t MEGABYTE = (1024 * KILOBYTE);
	static const size_t GIGABYTE = (1024 * MEGABYTE);
}

#endif // SIZE_HPP
