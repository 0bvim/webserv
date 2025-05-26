#include "gtest/gtest.h"
#include "URL.hpp"
#include "Connection.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <string>
#include <iostream>

class URLTestSimple : public ::testing::Test {
protected:
    void SetUp() override {
        server = new Server();

        Location loc;
        loc.setRoot("/var/www");
        loc.setURI("/");

        std::set<std::string> indexes;
        indexes.insert("index.html");
        loc.setIndexes(indexes);

        server->addLocation(loc);

        conn = new Connection(1, "127.0.0.1");
        conn->setHost("localhost:8080");

        std::string method = "GET";
        conn->setMethod(method);
        conn->setTarget("/test/path.html");
        conn->setServer(*server);
        conn->setLocation(loc);
    }

    void TearDown() override {
        delete conn;
        delete server;
    }

    Connection* conn;
    Server* server;
};

TEST_F(URLTestSimple, BasicURLParsing) {
    URL url(conn);

    EXPECT_EQ(url.getScheme(), "http");
    EXPECT_EQ(url.getHost(), "localhost");
    EXPECT_EQ(url.getPort(), "8080");
    EXPECT_EQ(url.getPath(), "/test/path.html");
    EXPECT_EQ(url.getFile(), "path.html");
    EXPECT_EQ(url.getExtension(), ".html");
}

TEST_F(URLTestSimple, PathFormatting) {
    conn->setTarget("/path/./to/../file.txt");
    URL url(conn);

    EXPECT_EQ(url.getPath(), "/path/file.txt");
}

TEST_F(URLTestSimple, QueryParameters) {
    conn->setTarget("/search.php?q=test&page=1");
    URL url(conn);

    EXPECT_EQ(url.getPath(), "/search.php");
    EXPECT_EQ(url.getQuery(), "q=test&page=1");
}
