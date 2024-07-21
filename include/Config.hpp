#ifndef CONFIG_HPP
#define CONFIG_HPP

# include "common.hpp"
# define DEFAULT_CONFIG_FILE "default.conf"

struct LocationConfig {
    std::string path;
    bool autoindex;
    std::vector<std::string> allow_methods;
    std::string root;
    std::string upload_dir;
    std::vector<std::string> index;
    std::string cgi_extension;
    std::string cgi_path;
    std::string redirect;
};

struct ServerConfig {
    std::string server_name;
    std::string listen;
    std::map<int, std::string> error_pages;
    size_t client_max_body_size;
    std::vector<LocationConfig> locations;
};

class Config {
    public:
        Config(const std::string &filePath);
        const std::vector<ServerConfig>& getServers() const;
        void printServers() const;

    private:
        std::string filePath;
        std::vector<ServerConfig> servers;

        void parseConfigFile();
        void parseServerBlock(const std::vector<std::string> &lines, size_t &index);
        void parseLocationBlock(const std::vector<std::string> &lines, size_t &index, ServerConfig &server);
        std::string trim(const std::string& str);
};

#endif
