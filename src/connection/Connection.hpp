#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "AStream.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "URL.hpp"
#include <ctime>
#include <map>
#include <string>
#include <vector>

class Resource;
class Http;

class Connection : public AStream {
	private:
		std::string _host;
		std::string _method;
		std::string _target;
		std::string _protocol;
		std::string _code;
		std::string _status;
		std::map<std::string, std::string> _headers;
		std::string _body;
		URL *_uri;
		Resource *_file;
		Server _server;
		Location _location;
		std::vector<Resource *> _garbage;

		void parseRequest(void);

	public:
		Connection(const int &fd, const std::string &ip);
		Connection(const Connection &src);
		Connection &operator=(const Connection &rhs);
		virtual ~Connection(void);

		void setHost(const std::string &host);
		std::string getHost(void) const;
		void processInput(const size_t &bytes);
		void setMethod(const std::string &method);
		std::string getMethod(void) const;
		void setTarget(const std::string &target);
		std::string getTarget(void) const;
		void setProtocol(const std::string &protocol);
		std::string getProtocol(void) const;
		void setCode(const std::string &code);
		std::string getCode(void) const;
		void setStatus(const std::string &status);
		std::string getStatus(void) const;
		void setTime(void);
		void addHeader(const std::string &key, const std::string &value);
		void addHeader(const std::string &key, const size_t &value);
		void setHeaders(const std::map<std::string, std::string> &headers);
		std::string getHeaderByKey(const std::string &key) const;
		std::map<std::string, std::string> getHeaders(void) const;
		std::size_t getHeadersSize(void) const;
		void addBody(const std::string &body);
		void setBody(const std::string &body);
		std::string getBody(void) const;
		void setUri(URL *uri);
		URL *getUri(void) const;
		void setResource(Resource *file);
		void setServer(const Server &server);
		Server &getServer(void);
		void setLocation(const Location &location);
		Location &getLocation(void);
		void buildResponse(void);
		void processOutput(const size_t &bytes);
		void resetConnection(void);
		bool isKeepAliveTimedOut(void) const;
		void sendTimeOut(void);

		std::string operator[](const std::string &key);
		bool operator==(const std::string &key);

};

std::ostream &operator<<(std::ostream &os, const Connection &src);

#endif // CONNECTION_HPP
