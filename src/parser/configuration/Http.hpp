#ifndef HTTP_HPP
#define HTTP_HPP

#include "Server.hpp"
#include "WebServ.hpp"
#include <bitset>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

class Http {
	private:
		static Http *_instance;

		std::string _error_log;
		std::string _root;
		std::bitset<2> _autoindex;
		std::bitset<2> _webdav;
		std::size_t _max_body_size;
		std::set<std::string> _indexes;
		std::map<std::string, std::string> _error_pages;
		std::vector<Server> _servers;
		int _signal;

	public:
		Http(void);
		~Http(void);

		static Http *getInstance(void);

		void configure(std::string configuration_file);
		void setRoot(std::string root);
		std::string getRoot(void) const;
		void setAutoIndex(std::string autoindex);
		void setAutoIndex(std::bitset<2> autoindex);
		std::bitset<2> getAutoIndexBitSet(void) const;
		bool getAutoIndex(void) const;
		void setWebDav(std::string webdav);
		void setWebDav(std::bitset<2> webdav);
		std::bitset<2> getWebDavBitSet(void) const;
		bool getWebDav(void) const;
		void setMaxBodySize(std::string max_body_size);
		std::size_t getMaxBodySize(void) const;
		void addIndex(std::string index);
		void setIndex(std::set<std::string> indexes);
		std::set<std::string> getIndexes(void) const;
		void addErrorPage(std::string error_page);
		void setErrorPages(std::map<std::string, std::string> error_pages);
		std::string getErrorPageByCode(std::string code) const;
		std::map<std::string, std::string> getErrorPages(void) const;
		void addServer(Server server);
		void setServers(std::vector<Server> servers);
		Server getServerByListen(std::string listen) const;
		Server getServerByName(std::string name) const;
		std::vector<Server> getServers(void) const;
		bool empty(void) const;
		void start(void);
		void stop(int signal);
		int getSignal(void);
};

std::ostream &operator<<(std::ostream &os, const Http &src);

#endif // HTTP_HPP
