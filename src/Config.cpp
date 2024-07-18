#include "Config.hpp"

Config::Config(const std::string& filePath) : filePath(filePath) {
    parseConfigFile();
}

void Config::parseConfigFile() {
    std::ifstream configFile(filePath.c_str());
    if (!configFile.is_open()) {
        std::cerr << "Error opening config file: " << filePath << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(configFile, line))
        lines.push_back(line);
    size_t index = 0;
    while (index < lines.size()) {
        std::string trimmedLine = trim(lines[index]);
        if (trimmedLine == "server {")
            parseServerBlock(lines, index);
        ++index;
    }
}

void Config::parseServerBlock(const std::vector<std::string>& lines, size_t& index) {
    ServerConfig server;

    while (++index < lines.size()) {
        std::string trimmedLine = trim(lines[index]);
        if (trimmedLine == "}")
            break;
        else if (trimmedLine.find("server_name") == 0)
            server.server_name = trimmedLine.substr(12);
        else if (trimmedLine.find("listen") == 0) {
            std::istringstream iss(trimmedLine.substr(7));
            iss >> server.listen;
        } 
        else if (trimmedLine.find("client_max_body_size") == 0) {
            std::istringstream iss(trimmedLine.substr(21));
            iss >> server.client_max_body_size;
        } 
        else if (trimmedLine.find("error_page") == 0) {
            size_t pos = trimmedLine.find(' ', 11);
            int errorCode;
            std::istringstream iss(trimmedLine.substr(11, pos - 11));
            iss >> errorCode;
            std::string errorPage = trimmedLine.substr(pos + 1);
            server.error_pages[errorCode] = errorPage;
        } 
        else if (trimmedLine.find("location") == 0)
            parseLocationBlock(lines, index, server);
    }

    servers.push_back(server);
}


void Config::parseLocationBlock(const std::vector<std::string>& lines, size_t& index, ServerConfig& server) {
    LocationConfig location;

    size_t pos = lines[index].find(' ');
    location.path = lines[index].substr(pos + 1);

    while (++index < lines.size()) {
        std::string trimmedLine = trim(lines[index]);
        if (trimmedLine == "}")
            break;
        else if (trimmedLine.find("autoindex") == 0)
            location.autoindex = (trimmedLine.substr(10) == "on");
        else if (trimmedLine.find("root") == 0)
            location.root = trimmedLine.substr(5);
        else if (trimmedLine.find("upload_dir") == 0)
            location.upload_dir = trimmedLine.substr(11);
        else if (trimmedLine.find("index") == 0) {
            std::istringstream iss(trimmedLine.substr(6));
            std::string idx;
            while (iss >> idx)
                location.index.push_back(idx);
        }
        else if (trimmedLine.find("allow_methods") == 0) {
            std::istringstream iss(trimmedLine.substr(14));
            std::string method;
            while (iss >> method)
                location.allow_methods.push_back(method);
        }
        else if (trimmedLine.find("cgi_extension") == 0)
            location.cgi_extension = trimmedLine.substr(14);
        else if (trimmedLine.find("cgi_path") == 0)
            location.cgi_path = trimmedLine.substr(9);
        else if (trimmedLine.find("redirect") == 0)
            location.redirect = trimmedLine.substr(9);
    }

    server.locations.push_back(location);
}

std::string Config::trim(const std::string& str) {
    size_t first = 0;
    size_t last = str.size() - 1;

    while (first <= last && std::isspace(str[first]))
        ++first;
    while (last >= first && std::isspace(str[last]))
        --last;
    if (first > last)
        return "";
    return str.substr(first, last - first + 1);
}

void Config::printServers() const {
    std::cout << "Number of servers: " << servers.size() << "\n\n";
    for (size_t i = 0; i < servers.size(); ++i) {
        const ServerConfig& server = servers[i];
        std::cout << "Server Name: " << server.server_name << "\n";
        std::cout << "Listen: " << server.listen << "\n";
        std::cout << "Client Max Body Size: " << server.client_max_body_size << "\n";

        for (std::map<int, std::string>::const_iterator it = server.error_pages.begin(); it != server.error_pages.end(); ++it) {
            std::cout << "Error Page " << it->first << ": " << it->second << "\n";
        }

        for (size_t j = 0; j < server.locations.size(); ++j) {
            const LocationConfig& location = server.locations[j];
            std::cout << "Location: " << location.path << "\n";
            std::cout << "  Autoindex: " << (location.autoindex ? "on" : "off") << "\n";
            std::cout << "  Root: " << location.root << "\n";
            std::cout << "  Upload Dir: " << location.upload_dir << "\n";
            std::cout << "  Index: ";
            for (size_t k = 0; k < location.index.size(); ++k)
                std::cout << location.index[k] << " ";
            std::cout << "\n";
            std::cout << "  Allow Methods: ";
            for (size_t k = 0; k < location.allow_methods.size(); ++k)
                std::cout << location.allow_methods[k] << " ";
            std::cout << "\n";
            if (!location.cgi_extension.empty()) {
                std::cout << "  CGI Extension: " << location.cgi_extension << "\n";
                std::cout << "  CGI Path: " << location.cgi_path << "\n";
            }
            if (!location.redirect.empty())
                std::cout << "  Redirect: " << location.redirect << "\n";
        }
        std::cout << "\n";
    }
}
