#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"
#include <bitset>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>

class Server {
	private:
		std::vector<std::string> _listen;
		std::vector<std::string> _names;
		std::string _root;
		std::bitset<2> _autoindex;
		std::bitset<2> _webdav;
		std::size_t _max_body_size;
		std::set<std::string> _indexes;
		std::map<std::string, std::string> _error_pages;
		std::map<std::string, Location> _locations;
		std::string _return_code;
		std::string _return_uri;

	public:
		Server(void);
		Server(std::string &configuration_file);
		Server(const Server &src);
		Server &operator=(const Server &rhs);
		~Server(void);

		bool operator<(const Server &rhs) const;

		void addListen(std::string listen);
		void setListen(std::vector<std::string> listen);
		std::vector<std::string> getListen(void) const;
		void addName(std::string name);
		void setNames(std::vector<std::string> names);
		std::vector<std::string> getNames(void) const;
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
		void setIndexes(std::set<std::string> indexes);
		std::set<std::string> getIndexes(void) const;
		void addErrorPage(std::string error_page);
		void setErrorPages(std::map<std::string, std::string> error_pages);
		std::string getErrorPageByCode(std::string code) const;
		std::map<std::string, std::string> getErrorPages(void) const;
		void addLocation(Location location);
		void setLocations(std::map<std::string, Location> locations);
		Location getLocationByURI(std::string uri) const;
		std::map<std::string, Location> getLocations(void) const;
		void setReturn(std::string value);
		std::string getReturnCode(void) const;
		std::string getReturnURI(void) const;
		bool empty(void) const;

};

std::ostream &operator<<(std::ostream &os, const Server &src);

#endif // SERVER_HPP
