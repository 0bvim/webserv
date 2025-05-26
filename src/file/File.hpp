#ifndef FILE_HPP
#define FILE_HPP

#include "Resource.hpp"
#include <fstream>

class Connection;

class File : public Resource
{
	private:
		std::ifstream _file;

		void processOutput(const size_t &bytes);

	public:
		File(Connection *connection);
		File(const File &src);
		File &operator=(const File &rhs);
		~File(void);
};

#endif // FILE_HPP
