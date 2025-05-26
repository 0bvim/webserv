#ifndef MOCK_PARSER_HPP
#define MOCK_PARSER_HPP

#include <gmock/gmock.h>
#include "parser.hpp"
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include <string>
#include <list>

class MockParser {
public:
    MOCK_METHOD(size_t, toSizeT, (std::string value), ());
    MOCK_METHOD(std::string, toString, (size_t value), ());
    MOCK_METHOD(std::string, toLower, (std::string text), ());
    MOCK_METHOD(void, replace, (std::string &buffer, char from, char to), ());
    MOCK_METHOD(void, erase, (std::string &buffer, std::string text, size_t quantity), ());
    MOCK_METHOD(void, rerase, (std::string &buffer, std::string text, size_t quantity), ());
    MOCK_METHOD(void, trim, (std::string &buffer, std::string set), ());
    MOCK_METHOD(bool, compare, (std::string key, std::string &buffer), ());
    MOCK_METHOD(std::string, find, (std::string key, std::string &buffer, std::string delimiter), ());
    MOCK_METHOD(std::list<std::string>, split, (std::string text, char delimiter), ());
    MOCK_METHOD(std::string, basename, (std::string text), ());
    MOCK_METHOD(void, http, (Http &http, std::string &buffer), ());
    MOCK_METHOD(void, server, (Server &server, std::string &buffer), ());
    MOCK_METHOD(void, location, (Location &location, std::string &buffer), ());
    MOCK_METHOD(void, limit_except, (Location &location, std::string &buffer), ());
};

#endif /* MOCK_PARSER_HPP */
