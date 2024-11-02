//
// Created by vinicius on 11/2/24.
//

// config_parser.hpp
#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <fstream>
#include "ConfigTypes.hpp"

class ConfigParser {
private:
    std::vector<Server> _servers;
    std::string _current_token;
    size_t _line_number;
    std::ifstream _file;

    // private methods
    void parseServer();
    Location parseLocation();
    void parseErrorPages(Server& server);
    std::string parseString();
    int parseNumber();
    std::vector<std::string> parseMethods();

    // utils methods
    void expectToken(const std::string& expected);
    std::string nextToken();
    void skipWhitespace();
    static bool isValidMethod(const std::string& method);
    void parseCGI(Location& loc);
    void parseUpload(Location& loc);
    void parseRedirect(Location& loc);
    size_t parseSize(const std::string& size_str) const;

public:
    ConfigParser();
    ~ConfigParser();

    std::vector<Server> parse(const std::string& filename);
    void validate() const;
};

#endif
