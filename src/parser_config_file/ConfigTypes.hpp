//
// Created by vinicius on 11/2/24.
//

#ifndef CONFIGTYPES_HPP
#define CONFIGTYPES_HPP

#include <string>
#include <vector>
#include <map>
#include <exception>

class ConfigParseError : public std::exception {
private:
    std::string _message;
    size_t _line;

public:
    ConfigParseError(const std::string& msg, size_t line_number)
        : _message(msg), _line(line_number) {}

    virtual ~ConfigParseError() throw() {}

    virtual const char* what() const throw() {
        return _message.c_str();
    }

    size_t getLine() const { return _line; }
};

struct CGI {
    bool enabled;
    std::string extension;
    std::string path;

    CGI() : enabled(false) {}
};

struct Upload {
    bool enabled;
    std::string path;
    size_t max_size;

    Upload() : enabled(false), max_size(0) {}
};

struct Redirect {
    bool enabled;
    int code;
    std::string url;

    Redirect() : enabled(false), code(301) {}
};

struct Location {
    std::string path;
    std::vector<std::string> methods;
    std::string root;
    bool directory_listing;
    std::string index_file;
    CGI cgi;
    Upload upload;
    Redirect redirect;
    size_t client_max_size;

    Location() : directory_listing(false), client_max_size(0) {}
};

struct Server {
    std::string host;
    int port;
    std::vector<std::string> server_names;
    size_t client_max_size;
    std::map<int, std::string> error_pages;
    std::vector<Location> locations;

    Server() : port(0), client_max_size(0) {}
};

#endif