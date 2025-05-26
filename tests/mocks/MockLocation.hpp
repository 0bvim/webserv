#ifndef MOCK_LOCATION_HPP
#define MOCK_LOCATION_HPP

#include "Location.hpp"
#include "gmock/gmock.h"
#include <string>
#include <set>
#include <map>
#include <bitset>

class MockLocation : public Location {
public:
    MockLocation() : Location() {}
    
    MOCK_METHOD(std::string, getURI, (), (const));
    MOCK_METHOD(std::string, getMethod, (std::string method), (const));
    MOCK_METHOD((std::set<std::string>), getMethods, (), (const));
    MOCK_METHOD(bool, getDenyMethods, (), (const));
    MOCK_METHOD(std::string, getRoot, (), (const));
    MOCK_METHOD((std::bitset<2>), getAutoIndexBitSet, (), (const));
    MOCK_METHOD(bool, getAutoIndex, (), (const));
    MOCK_METHOD(std::size_t, getMaxBodySize, (), (const));
    MOCK_METHOD((std::set<std::string>), getIndexes, (), (const));
    MOCK_METHOD(std::string, getFastCgi, (), (const));
    MOCK_METHOD((std::map<std::string, std::string>), getErrorPages, (), (const));
    MOCK_METHOD(std::string, getErrorPageByCode, (std::string code), (const));
    MOCK_METHOD(std::string, getReturnCode, (), (const));
    MOCK_METHOD(std::string, getReturnURI, (), (const));
    MOCK_METHOD(bool, empty, (), (const));
};

#endif // MOCK_LOCATION_HPP 