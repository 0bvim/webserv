#ifndef MOCK_CONNECTION_HPP
#define MOCK_CONNECTION_HPP

#include "Connection.hpp"
#include "gmock/gmock.h"
#include "Resource.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <map>

class MockConnection : public Connection {
public:
    MockConnection(int id, const std::string& host) : Connection(id, host) {}

    MOCK_METHOD(void, setCode, (const std::string& code));
    MOCK_METHOD(void, setStatus, (const std::string& status));
    MOCK_METHOD(void, setResource, (Resource* resource));
    MOCK_METHOD(void, buildResponse, ());
    MOCK_METHOD(std::string, getCode, (), (const));
    MOCK_METHOD(std::string, getStatus, (), (const));

    // Additional methods for more comprehensive testing
    MOCK_METHOD(std::string, getHost, (), (const));
    MOCK_METHOD(std::string, getMethod, (), (const));
    MOCK_METHOD(std::string, getTarget, (), (const));
    MOCK_METHOD(std::string, getProtocol, (), (const));
    MOCK_METHOD(std::string, getId, (), (const));
    MOCK_METHOD(std::string, getHeaderByKey, (std::string key), (const));
    MOCK_METHOD(void, setProtocol, (std::string protocol));
    MOCK_METHOD(void, setHeaders, ((const std::map<std::string, std::string>& headers)));
    MOCK_METHOD(void, addHeader, (std::string key, std::string value));
    MOCK_METHOD(void, setTime, ());
    MOCK_METHOD(const Location&, getLocation, (), (const));
    MOCK_METHOD(void, setLocation, (const Location& location));
    MOCK_METHOD(Server&, getServer, ());
    MOCK_METHOD(void, setServer, (Server server));
};

#endif // MOCK_CONNECTION_HPP
