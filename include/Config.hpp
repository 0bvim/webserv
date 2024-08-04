#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "common.hpp"
#define DEFAULT_CONFIG_FILE "default.conf"

typedef struct cgi_config
{
	std::string extension;
	std::string path;
} t_cgi_config;

struct LocationConfig
{
	std::string path;
	bool autoindex;
	std::vector<std::string> allow_methods;
	std::string root;
	std::string upload_dir;
	std::vector<std::string> index;
	std::vector<t_cgi_config> cgi;
	std::string redirect;
};

typedef struct ServerConfig
{
	std::string server_name;
	std::string listen;
	std::map<int, std::string> error_pages;
	size_t client_max_body_size;
	std::vector<LocationConfig> locations;
} t_ServerConfig;

class Config
{
public:
	Config(const std::string &filePath);
	const std::vector<ServerConfig> &getServers() const;
	const std::string getServerAddress() const;
	int getServerPort() const;
	void printServers() const;

private:
	std::string filePath;
	std::vector<ServerConfig> servers;

	void parseConfigFile();
	void parseServerBlock(const std::vector<std::string> &lines, size_t &index);
	void parseLocationBlock(const std::vector<std::string> &lines, size_t &index, ServerConfig &server);
	bool checkBraces(const std::vector<std::string> &lines);
};

#endif
