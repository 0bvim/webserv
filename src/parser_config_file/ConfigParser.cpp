//
// Created by vinicius on 11/2/24.
//

#include "ConfigParser.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cctype>

std::string ConfigParser::nextToken()
{
    skipWhitespace();

    if (_file.eof())
        return "";

    std::string token;
    char c;

    _file.get(c);
    if (c == '{' || c == '}' || c == ';')
    {
        token = c;
        return token;
    }

    _file.unget();
    while (_file.get(c) && !std::isspace(c) && c != '{' && c != '}' && c != ';')
        token += c;

    if (!std::isspace(c))
        _file.unget();

    return token;
}

void ConfigParser::skipWhitespace() {
    char c;

    while (_file.get(c))
    {
        if (c == '#')
            while (_file.get(c) && c != '\n') {}
        if (c == '\n')
            _line_number++;
        if (!std::isspace(c))
        {
            _file.unget();
            break;
        }
    }
}

void ConfigParser::expectToken(const std::string& expected)
{
    const std::string token = nextToken();

    if (token != expected)
    {
        const std::string error = "Expected '" + expected + "', got '" + token + "'";
        throw ConfigParseError(error, _line_number);
    }
}

std::string ConfigParser::parseString() { return nextToken(); }

int ConfigParser::parseNumber()
{
    const std::string token = nextToken();
    std::istringstream iss(token);
    int number;
    if (!(iss >> number))
        throw ConfigParseError("Invalid number", _line_number);
    return number;
}

size_t ConfigParser::parseSize(const std::string& size_str) const
{
    const size_t len = size_str.length();
    if (len < 2)
        throw ConfigParseError("Invalid size format", _line_number);

    const char unit = std::toupper(size_str[len - 1]);
    const std::string number_str = size_str.substr(0, len - 1);
    const size_t base = std::atol(number_str.c_str());

    switch (unit)
    {
        case 'K': return base * 1024;
        case 'M': return base * 1024 * 1024;
        case 'G': return base * 1024 * 1024 * 1024;
        default:
            throw ConfigParseError("Invalid size unit (use K, M, or G)", _line_number);
    }
}

ConfigParser::ConfigParser() : _line_number() {}

ConfigParser::~ConfigParser() {}

std::vector<std::string> ConfigParser::parseMethods()
{
    std::vector<std::string> methods;
    std::string method;

    while ((method = nextToken()) != ";")
    {
        if (!isValidMethod(method))
            throw ConfigParseError("Invalid HTTP method: " + method, _line_number);
        methods.push_back(method);
    }
    _file.unget(); // Push back the semicolon
    return methods;
}

void ConfigParser::parseErrorPages(Server& server)
{
    expectToken("{");
    std::string token;

    while ((token = nextToken()) != "}")
    {
        if (isdigit(token[0]))
        {
            int code = std::atoi(token.c_str());
            const std::string path = parseString();
            server.error_pages[code] = path;
            expectToken(";");
        }
        else
            throw ConfigParseError("Expected error code", _line_number);
    }
}

bool ConfigParser::isValidMethod(const std::string& method)
{
    return method == "GET" ||
           method == "POST" ||
           method == "DELETE";
}

void ConfigParser::parseCGI(Location& loc) {
    expectToken("{");
    std::string token;

    while ((token = nextToken()) != "}")
    {
        if (token == "extension")
        {
            loc.cgi.extension = parseString();
            expectToken(";");
        }
        else if (token == "path")
        {
            loc.cgi.path = parseString();
            expectToken(";");
        }
    }
    loc.cgi.enabled = true;
}

void ConfigParser::parseUpload(Location& loc) {
    expectToken("{");
    std::string token;

    while ((token = nextToken()) != "}")
    {
        if (token == "path")
        {
            loc.upload.path = parseString();
            expectToken(";");
        }
        else if (token == "max_size")
        {
            std::string size_str = parseString();
            loc.upload.max_size = parseSize(size_str);
            expectToken(";");
        }
    }
    loc.upload.enabled = true;
}

void ConfigParser::parseRedirect(Location& loc)
{
    expectToken("{");
    std::string token;

    while ((token = nextToken()) != "}")
    {
        if (token == "code")
        {
            loc.redirect.code = parseNumber();
            expectToken(";");
        }
        else if (token == "url")
        {
            loc.redirect.url = parseString();
            expectToken(";");
        }
    }
    loc.redirect.enabled = true;
}

void ConfigParser::parseServer()
{
    Server server;
    expectToken("{");

    std::string token;
    while ((token = nextToken()) != "}")
    {
        if (token == "host")
        {
            server.host = parseString();
            expectToken(";");
        }
        else if (token == "port")
        {
            server.port = parseNumber();
            expectToken(";");
        }
        else if (token == "location")
            server.locations.push_back(parseLocation());
        else if (token == "error_pages")
            parseErrorPages(server);
        else if (token == "server_names")
        {
            std::string name;
            while ((name = parseString()) != ";")
                server.server_names.push_back(name);
        }
        else if (token == "client_max_size")
        {
            std::string size_str = parseString();
            server.client_max_size = parseSize(size_str);
            expectToken(";");
        }
    }

    _servers.push_back(server);
}
std::vector<Server> ConfigParser::parse(const std::string& filename)
{
    _file.open(filename.c_str());
    if (!_file.is_open())
        throw ConfigParseError("Cannot open config file", 0);
    while (_file.good())
    {
        std::string token = nextToken();
        if (token.empty())
            break;
        if (token == "server")
            parseServer();
        else
            throw ConfigParseError("Expected 'server' block", _line_number);
    }
    validate();
    return _servers;
}

void ConfigParser::validate() const {
    if (_servers.empty())
        throw ConfigParseError("No server blocks found", 0);

    for (std::vector<Server>::const_iterator it = _servers.begin(); it != _servers.end(); ++it)
    {
        if (it->host.empty())
            throw ConfigParseError("Server host not specified", 0);
        if (it->port <= 0 || it->port > 65535)
            throw ConfigParseError("Invalid port number", 0);
        if (it->locations.empty())
            throw ConfigParseError("No locations defined for server", 0);
    }
}

Location ConfigParser::parseLocation()
{
    Location loc;
    loc.path = parseString();
    expectToken("{");

    std::string token;
    while ((token = nextToken()) != "}")
    {
        if (token == "methods")
        {
            loc.methods = parseMethods();
            expectToken(";");
        }
        else if (token == "root")
        {
            loc.root = parseString();
            expectToken(";");
        }
        else if (token == "directory_list")
        {
            std::string value = parseString();
            loc.directory_listing = (value == "on");
            expectToken(";");
        }
        else if (token == "index")
        {
            loc.index_file = parseString();
            expectToken(";");
        }
        else if (token == "cgi")
            parseCGI(loc);
        else if (token == "upload")
            parseUpload(loc);
        else if (token == "redirect")
            parseRedirect(loc);
        else if (token == "client_max_size")
        {
            std::string size_str = parseString();
            loc.client_max_size = parseSize(size_str);
            expectToken(";");
        }
    }

    return loc;
}
