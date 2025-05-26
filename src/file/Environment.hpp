#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "Cgi.hpp"
#include <vector>

class Environment
{
	private:
		std::vector<char *> _env;

		void addEnv(const std::string &key, const std::string &value);
	
	public:
		Environment(Connection *Connection);
		Environment(const Environment &src);
		Environment &operator=(const Environment &rhs);
		~Environment(void);

		std::vector<char *> getEnvironment(void);
};

#endif // ENVIRONMENT_HPP
