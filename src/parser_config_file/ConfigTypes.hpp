//
// Created by vinicius on 11/2/24.
//

#ifndef CONFIGTYPES_HPP
#define CONFIGTYPES_HPP

#include <string>
#include <vector>
#include <map>
#include <exception>

class ConfigParseError : public std::exception
{
private:
    std::string _message;
    size_t _line;

public:
    ConfigParseError(const std::string& msg, size_t line_number)
        : _message(msg), _line(line_number)
    {
    }

    virtual ~ConfigParseError() throw()
    {
    }

    virtual const char* what() const throw()
    {
        return _message.c_str();
    }

    size_t getLine() const { return _line; }
};

struct CGI
{
    bool enabled;
    std::string extension;
    std::string path;

    CGI() : enabled(false)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const CGI& cgi)
    {
        os << "CGI { enabled: " << (cgi.enabled ? "true" : "false")
            << ", extension: " << cgi.extension
            << ", path: " << cgi.path << " }";
        return os;
    }
};

struct Upload
{
    bool enabled;
    std::string path;
    size_t max_size;

    Upload() : enabled(false), max_size(0)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Upload& upload)
    {
        os << "Upload { enabled: " << (upload.enabled ? "true" : "false")
            << ", path: " << upload.path
            << ", max_size: " << upload.max_size << " }";
        return os;
    }
};

struct Redirect
{
    bool enabled;
    int code;
    std::string url;

    Redirect() : enabled(false), code(301)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Redirect& redirect)
    {
        os << "Redirect { enabled: " << (redirect.enabled ? "true" : "false")
            << ", code: " << redirect.code
            << ", url: " << redirect.url << " }";
        return os;
    }
};

struct Location
{
    std::string path;
    std::vector<std::string> methods;
    std::string root;
    bool directory_listing;
    std::string index_file;
    CGI cgi;
    Upload upload;
    Redirect redirect;
    size_t client_max_size;

    Location() : directory_listing(false), client_max_size(0)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Location& location)
    {
        os << "Location {\n"
            << "  path: " << location.path << "\n"
            << "  methods: [";
        for (size_t i = 0; i < location.methods.size(); ++i)
        {
            os << location.methods[i];
            if (i < location.methods.size() - 1) os << ", ";
        }
        os << "]\n"
            << "  root: " << location.root << "\n"
            << "  directory_listing: " << (location.directory_listing ? "true" : "false") << "\n"
            << "  index_file: " << location.index_file << "\n"
            << "  cgi: " << location.cgi << "\n"
            << "  upload: " << location.upload << "\n"
            << "  redirect: " << location.redirect << "\n"
            << "  client_max_size: " << location.client_max_size << "\n"
            << "}";
        return os;
    }
};

struct Server
{
    std::string host;
    int port;
    std::vector<std::string> server_names;
    size_t client_max_size;
    std::map<int, std::string> error_pages;
    std::vector<Location> locations;

    Server() : port(0), client_max_size(0)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Server& server)
    {
        os << "Server {\n"
            << "  host: " << server.host << "\n"
            << "  port: " << server.port << "\n"
            << "  server_names: [";
        for (size_t i = 0; i < server.server_names.size(); ++i)
        {
            os << server.server_names[i];
            if (i < server.server_names.size() - 1) os << ", ";
        }
        os << "]\n"
            << "  client_max_size: " << server.client_max_size << "\n"
            << "  error_pages: {";
        for (std::map<int, std::string>::const_iterator it = server.error_pages.begin(); it != server.error_pages.end(); ++it)
            os << " " << it->first << ": " << it->second << ",";
        os << " }\n"
            << "  locations: [";
        for (size_t i = 0; i < server.locations.size(); ++i)
        {
            os << server.locations[i];
            if (i < server.locations.size() - 1) os << ", ";
        }
        os << "]\n"
            << "}";
        return os;
    }
};

#endif
