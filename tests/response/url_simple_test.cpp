#include "gtest/gtest.h"
#include "URL.hpp"
#include "Connection.hpp"
#include "Location.hpp"
#include <string>
#include <iostream>

class URLSimpleTest : public ::testing::Test {
protected:
    void SetUp() override {
        conn = new Connection(1, "127.0.0.1");

        conn->setHost("localhost:8080");

        std::string method = "GET";
        conn->setMethod(method);

        conn->setTarget("/test/path.html");

        Location loc;
        loc.setRoot("/var/www");

        std::set<std::string> indexes;
        indexes.insert("index.html");
        loc.setIndexes(indexes);

        conn->setLocation(loc);

        std::cout << "Connection host: " << conn->getHost() << std::endl;
        std::cout << "Connection path: " << conn->getTarget() << std::endl;
    }

    void TearDown() override {
        delete conn;
    }

    Connection* conn;
};

TEST_F(URLSimpleTest, BasicURLConstruction) {
    URL url(conn);

    std::cout << "URL host: " << url.getHost() << std::endl;
    std::cout << "URL port: " << url.getPort() << std::endl;
    std::cout << "URL path: " << url.getPath() << std::endl;
    std::cout << "URL file: " << url.getFile() << std::endl;

    EXPECT_FALSE(url.getHost().empty());
    EXPECT_FALSE(url.getPort().empty());
}
