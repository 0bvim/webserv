#include "gtest/gtest.h"
#include "URL.hpp"
#include "Connection.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <string>
#include <set>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

class URLTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        connection = new Connection(1, "127.0.0.1");

        server.setRoot("/var/www/html");
        std::set<std::string> indexes;
        indexes.insert("index.html");
        server.setIndexes(indexes);

        location.setRoot("/var/www/html");
        location.setURI("/");
        location.setIndexes(indexes);

        std::map<std::string, Location> locations;
        locations["/"] = location;
        server.setLocations(locations);

        connection->setServer(server);
        connection->setLocation(location);
        connection->setHost("localhost:8080");
        connection->setTarget("/");

        std::string method = "GET";
        connection->setMethod(method);
    }

    void TearDown() override
    {
        delete connection;
    }

    Connection *connection;
    Server server;
    Location location;
};

TEST_F(URLTest, ConstructSimpleURL)
{
    connection->setTarget("/index.html");

    URL url(connection);

    EXPECT_EQ(url.getScheme(), "http");
    EXPECT_EQ(url.getHost(), "localhost");
    EXPECT_EQ(url.getPort(), "8080");
    EXPECT_STREQ(url.getPath().c_str(), "/index.html");
    EXPECT_STREQ(url.getFile().c_str(), "index.html");
    EXPECT_STREQ(url.getExtension().c_str(), ".html");
    EXPECT_EQ(url.getQuery(), "");
}

TEST_F(URLTest, ConstructURLWithQuery)
{
    connection->setTarget("/search.php?q=test&page=1");

    URL url(connection);

    EXPECT_EQ(url.getScheme(), "http");
    EXPECT_EQ(url.getHost(), "localhost");
    EXPECT_EQ(url.getPort(), "8080");
    EXPECT_STREQ(url.getPath().c_str(), "/search.php");
    EXPECT_STREQ(url.getFile().c_str(), "search.php");
    EXPECT_STREQ(url.getExtension().c_str(), ".php");
    EXPECT_EQ(url.getQuery(), "q=test&page=1");
}

TEST_F(URLTest, PathFormatting)
{
    connection->setTarget("/path/./to/../file.txt");

    URL url(connection);

    EXPECT_STREQ(url.getPath().c_str(), "/path/file.txt");
    EXPECT_STREQ(url.getFile().c_str(), "file.txt");
}

TEST_F(URLTest, PercentEncodedCharacters)
{
    connection->setTarget("/path/with%20space%21.txt");

    URL url(connection);

    EXPECT_STREQ(url.getPath().c_str(), "/path/with%20space%21.txt");
    EXPECT_STREQ(url.getFile().c_str(), "with%20space%21.txt");
}

TEST_F(URLTest, CopyAndAssignment)
{
    connection->setTarget("/index.html");

    URL url1(connection);
    URL url2(url1);
    URL url3 = url1;

    EXPECT_EQ(url2.getHost(), url1.getHost());
    EXPECT_EQ(url2.getPath(), url1.getPath());
    EXPECT_EQ(url2.getFile(), url1.getFile());

    EXPECT_EQ(url3.getScheme(), url1.getScheme());
    EXPECT_EQ(url3.getHost(), url1.getHost());
    EXPECT_EQ(url3.getPath(), url1.getPath());
    EXPECT_EQ(url3.getFile(), url1.getFile());
}

TEST_F(URLTest, DirectoryAndPermissions)
{
    system("mkdir -p /tmp/testdir");
    system("touch /tmp/testdir/index.html");
    system("chmod 755 /tmp/testdir");
    system("chmod 644 /tmp/testdir/index.html");

    Server tmpServer;
    tmpServer.setRoot("/tmp");

    Location tmpLocation;
    tmpLocation.setRoot("/tmp");
    tmpLocation.setURI("/");
    std::set<std::string> indexes;
    indexes.insert("index.html");
    tmpLocation.setIndexes(indexes);

    std::map<std::string, Location> locations;
    locations["/"] = tmpLocation;
    tmpServer.setLocations(locations);

    connection->setServer(tmpServer);
    connection->setLocation(tmpLocation);

    connection->setTarget("/testdir/index.html");

    URL fileUrl(connection);

    std::cout << "Absolute path: " << fileUrl.getAbsolutePath() << std::endl;
    std::cout << "Is file: " << (fileUrl.isFile() ? "true" : "false") << std::endl;
    std::cout << "Is directory: " << (fileUrl.isDirectory() ? "true" : "false") << std::endl;

    system("rm -rf /tmp/testdir");

    EXPECT_TRUE(fileUrl.isFile());
    EXPECT_TRUE(fileUrl.isReadable());
}

TEST_F(URLTest, HttpMethods)
{
    connection->setTarget("/index.html");
    std::string getMethod = "GET";
    connection->setMethod(getMethod);

    URL getUrl(connection);

    std::string deleteMethod = "DELETE";
    connection->setMethod(deleteMethod);

    URL deleteUrl(connection);

    EXPECT_NE(deleteUrl.getPath(), "");
}

TEST_F(URLTest, LocationDetermination)
{
    Location subLocation;
    subLocation.setRoot("/var/www/html");
    subLocation.setURI("/subdir");

    std::map<std::string, Location> locations = server.getLocations();
    locations["/subdir"] = subLocation;
    server.setLocations(locations);

    connection->setServer(server);
    connection->setTarget("/subdir/page.html");

    URL url(connection);

    EXPECT_STREQ(url.getLocation().c_str(), "http://localhost:8080/subdir/");
    EXPECT_STREQ(url.getAbsolutePath().c_str(), "/var/www/html/subdir/page.html");
}
