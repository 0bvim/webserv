#ifndef CGI_HPP
#define CGI_HPP

#include "Resource.hpp"
#include <string>
#include <vector>

class Connection;

class Cgi : public Resource
{
	private:
		int _sock[2];
		int _status;
		pid_t _pid;

		std::vector<char *> createVector(std::vector<std::string> &container);
		void addEnv(const std::string &key, const std::string &value);
		void populateEnv(Connection *connection);
 		void deleteVector(std::vector<char *> &container);
		void closeSockets(void);
		void processInput(const size_t &bytes);
		void parseCgiResponse(void);

	public:
		Cgi(Connection *connection);
		Cgi(const Cgi &src);
		Cgi &operator=(const Cgi &rhs);
		~Cgi(void);

		void sendCGI(void);
};

#endif // CGI_HPP
