#ifndef MOCK_SERVER_HPP
#define MOCK_SERVER_HPP

#include "Server.hpp"
#include "gmock/gmock.h"
#include <string>
#include <vector>
#include <set>

class MockServer : public Server {
public:
    MockServer() : Server() {}
    
    MOCK_METHOD(std::string, getHost, (), (const));
    MOCK_METHOD(std::string, getPort, (), (const));
    MOCK_METHOD(std::string, getName, (), (const));
    MOCK_METHOD(std::string, getRoot, (), (const));
    MOCK_METHOD((std::set<std::string>), getIndex, (), (const));
    MOCK_METHOD(std::string, getIndex, (std::string name), (const));
    MOCK_METHOD((std::set<std::string>), getMethods, (), (const));
    MOCK_METHOD(Location, getLocationByURI, (std::string uri), (const));
};

#endif // MOCK_SERVER_HPP 