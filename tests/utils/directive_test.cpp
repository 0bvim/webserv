#include "directive.hpp"
#include "parser.hpp"
#include "size.hpp"
#include <gtest/gtest.h>
#include <set>
#include <stdexcept>
#include <bitset>
#include <map>
#include <limits>
#include <vector>
#include <iostream>
#include "test_helpers.hpp"

using namespace std;

TEST(DirectiveTest, ValidateHttpListen) {

  EXPECT_FALSE(directive::validateHttpListen(""));
  EXPECT_TRUE(directive::validateHttpListen("127.0.0.1:8080"));
  EXPECT_TRUE(directive::validateHttpListen("192.168.1.1:80"));
  EXPECT_FALSE(directive::validateHttpListen("[::1]:8080"));
  EXPECT_FALSE(directive::validateHttpListen(
        "[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:80"));
  EXPECT_FALSE(directive::validateHttpListen("127.0.0.1:abc"));
  EXPECT_FALSE(directive::validateHttpListen("127.0.0.1:8080a"));
  EXPECT_FALSE(directive::validateHttpListen("127.0.0.1:8080:"));
  EXPECT_FALSE(directive::validateHttpListen("127.0.0.1:8080 "));
  EXPECT_FALSE(directive::validateHttpListen(":8080"));
  EXPECT_FALSE(directive::validateHttpListen("127.0.0.1:"));
  EXPECT_FALSE(directive::validateHttpListen("127.0.0.1::8080"));
  EXPECT_FALSE(directive::validateHttpListen("127.0.0.1:8080:extra"));
  EXPECT_TRUE(directive::validateHttpListen("8080"));
  EXPECT_FALSE(directive::validateHttpListen(":8080"));
  EXPECT_FALSE(directive::validateHttpListen("8080:"));
  EXPECT_FALSE(directive::validateHttpListen("[::1]::8080"));
  EXPECT_FALSE(directive::validateHttpListen("[::1]:8080:extra"));
}

TEST(DirectiveTest, ValidateHttpHost) {

  EXPECT_TRUE(directive::validateHttpHost(string("192.168.1.1")));
  EXPECT_TRUE(directive::validateHttpHost(string("10.0.0.1")));
  EXPECT_TRUE(directive::validateHttpHost(string("255.255.255.255")));
  EXPECT_FALSE(directive::validateHttpHost(string("192.168.1.a")));
  EXPECT_FALSE(directive::validateHttpHost(string("192.168.1.1 ")));
  EXPECT_FALSE(directive::validateHttpHost(string("192.168.1.1.")));
  EXPECT_FALSE(directive::validateHttpHost(string("192..168.1.1")));
  EXPECT_FALSE(directive::validateHttpHost(string(".192.168.1.1")));
  EXPECT_FALSE(directive::validateHttpHost(string("192.168.1.1.")));
  EXPECT_FALSE(directive::validateHttpHost(string("192.168.1")));
  EXPECT_FALSE(directive::validateHttpHost(string("192.168.1.1.1")));
  EXPECT_FALSE(directive::validateHttpHost(string("256.168.1.1")));
  EXPECT_FALSE(directive::validateHttpHost(string("192.168.256.1")));
  EXPECT_FALSE(directive::validateHttpHost(string("192.168.1.256")));
  EXPECT_FALSE(directive::validateHttpHost(string("")));
}

TEST(DirectiveTest, ValidateHttpPort) {

  EXPECT_TRUE(directive::validateHttpPort(string("8080")));
  EXPECT_TRUE(directive::validateHttpPort(string("1024")));
  EXPECT_TRUE(directive::validateHttpPort(string("49151")));

  EXPECT_FALSE(directive::validateHttpPort(string("")));
  EXPECT_FALSE(directive::validateHttpPort(string("65536")));
  EXPECT_FALSE(directive::validateHttpPort(string("123456")));
  EXPECT_FALSE(directive::validateHttpPort(string("-123456")));

  EXPECT_FALSE(directive::validateHttpPort(string("")));

  EXPECT_FALSE(directive::validateHttpPort(string("8080a")));
  EXPECT_FALSE(directive::validateHttpPort(string("80 80")));
  EXPECT_FALSE(directive::validateHttpPort(string("80.80")));

  EXPECT_FALSE(directive::validateHttpPort(string(" 8080 ")));
  EXPECT_FALSE(directive::validateHttpPort(string("\t8080\t")));
}

TEST(DirectiveTest, AddListen) {

  vector<string> listenList;

  EXPECT_NO_THROW(directive::addListen(string("192.168.1.1:8080"), listenList));
  EXPECT_EQ(listenList.back(), "192.168.1.1:8080");
  EXPECT_NO_THROW(directive::addListen(string("8080"), listenList));
  EXPECT_EQ(listenList.back(), "0.0.0.0:8080");
  EXPECT_NO_THROW(directive::addListen(string("192.168.1.2"), listenList));
  EXPECT_EQ(listenList.back(), "192.168.1.2:8080");
  EXPECT_NO_THROW(directive::addListen(string(""), listenList));
  EXPECT_EQ(listenList.size(), 3);
  EXPECT_THROW(directive::addListen(string("192.168.1.1:8080a"), listenList),
      runtime_error);
  EXPECT_THROW(directive::addListen(string("256.256.256.256:8080"), listenList),
      runtime_error);
  EXPECT_THROW(directive::addListen(string("192.168.1.1:65536"), listenList),
      runtime_error);
  EXPECT_THROW(directive::addListen(string("192.168.1.1:8080"), listenList),
      runtime_error);
}

TEST(DirectiveTest, ValidateName) {

  EXPECT_TRUE(directive::validateName(string("example")));
  EXPECT_TRUE(directive::validateName(string("example123")));
  EXPECT_TRUE(directive::validateName(string("example-name")));
  EXPECT_TRUE(directive::validateName(string("example.name")));
  EXPECT_FALSE(directive::validateName(string("")));
  EXPECT_FALSE(directive::validateName(string("example name")));
  EXPECT_FALSE(directive::validateName(string("example@name")));
  EXPECT_FALSE(directive::validateName(string("example!name")));
  EXPECT_FALSE(directive::validateName(string("example_name")));
  EXPECT_FALSE(directive::validateName(string("example#name")));
  EXPECT_FALSE(directive::validateName(string("example!name")));
  EXPECT_FALSE(directive::validateName(string("-example")));
  EXPECT_FALSE(directive::validateName(string(".example")));
  EXPECT_FALSE(directive::validateName(string("example-")));
  EXPECT_FALSE(directive::validateName(string("example.")));
  EXPECT_FALSE(directive::validateName(string("example-name@domain")));
  EXPECT_FALSE(directive::validateName(string("example.name with space")));
}

TEST(DirectiveTest, AddName) {

  vector<string> nameList;

  directive::addName("", nameList);
  EXPECT_TRUE(nameList.empty()) << "Expected empty vector for empty input";

  directive::addName("example", nameList);
  ASSERT_EQ(nameList.size(), 1) << "Expected one element";
  EXPECT_EQ(nameList[0], "example") << "Expected 'example'";

  directive::addName("example.com my-site 123server", nameList);
  ASSERT_EQ(nameList.size(), 4) << "Expected 4 elements";
  EXPECT_EQ(nameList[1], "example.com");
  EXPECT_EQ(nameList[2], "my-site");
  EXPECT_EQ(nameList[3], "123server");

  EXPECT_THROW(directive::addName("validname @invalid! anothervalid", nameList),
      runtime_error)
    << "Expected exception for invalid name";
  EXPECT_EQ(nameList[4], "validname");

  directive::addName("Example.COM My-SITE", nameList);
  EXPECT_EQ(nameList[5], "example.com");
  EXPECT_EQ(nameList[6], "my-site");
  ASSERT_EQ(nameList.size(), 7) << "Expected 6 elements";
}

TEST(DirectiveTest, SetURI) {

  string result = "unchanged";

  directive::setURI("", result);
  EXPECT_EQ(result, "unchanged");

  EXPECT_THROW(directive::setURI("invalid uri", result), runtime_error);

  directive::setURI("/valid/path", result);
  EXPECT_EQ(result, "/valid/path");

}

TEST(DirectiveTest, validateHttpMethod) {

  EXPECT_TRUE(directive::validateHttpMethod("GET"));
  EXPECT_TRUE(directive::validateHttpMethod("POST"));
  EXPECT_TRUE(directive::validateHttpMethod("DELETE"));
  EXPECT_FALSE(directive::validateHttpMethod("PUT"));
  EXPECT_FALSE(directive::validateHttpMethod("PATCH"));
}

TEST(DirectiveTest, addMethod) {

  set<string> allow_methods = {"GET POST DELETE"};

  EXPECT_THROW(directive::addMethod("MATCH", allow_methods), runtime_error);
  EXPECT_THROW(directive::addMethod("PATCH", allow_methods), runtime_error);
  EXPECT_THROW(directive::addMethod("PUT", allow_methods), runtime_error);
  EXPECT_THROW(directive::addMethod("PUT", allow_methods), runtime_error);
  EXPECT_NO_THROW(directive::addMethod("", allow_methods));
}

TEST(DirectiveTest, setDenyMethods) {

  bool denyMethod = false;

  EXPECT_NO_THROW(directive::setDenyMethods("", denyMethod));

  directive::setDenyMethods("all", denyMethod);
  EXPECT_TRUE(denyMethod);

  EXPECT_THROW(directive::setDenyMethods("anything", denyMethod), runtime_error);
}


TEST(DirectiveTest, setRoot) {

  std::string root;

  EXPECT_NO_THROW(directive::setRoot("", root));
  EXPECT_TRUE(root.empty());
  EXPECT_NO_THROW(directive::setRoot("/path/to/root", root));
  EXPECT_EQ(root, "/path/to/root");
  EXPECT_THROW(directive::setRoot("invalid path", root), std::runtime_error);
  EXPECT_NO_THROW(directive::setRoot("/another/path", root));
  EXPECT_EQ(root, "/another/path");
  EXPECT_NO_THROW(directive::setRoot("/path-with_special.chars", root));
  EXPECT_EQ(root, "/path-with_special.chars");
}

TEST(DirectiveTest, setAutoIndex) {

  std::bitset<2> autoindex;

  EXPECT_NO_THROW(directive::setAutoIndex("", autoindex));
  EXPECT_EQ(autoindex.to_ulong(), 0);
  EXPECT_NO_THROW(directive::setAutoIndex("on", autoindex));
  EXPECT_EQ(autoindex.to_ulong(), parser::AUTOINDEX_ON);
  EXPECT_NO_THROW(directive::setAutoIndex("off", autoindex));
  EXPECT_EQ(autoindex.to_ulong(), parser::AUTOINDEX_OFF);
  EXPECT_THROW(directive::setAutoIndex("invalid", autoindex), std::runtime_error);
  EXPECT_THROW(directive::setAutoIndex("ON", autoindex), std::runtime_error);
  EXPECT_THROW(directive::setAutoIndex("OFF", autoindex), std::runtime_error);
}

TEST(DirectiveTest, setMaxBodySize) {

  size_t maxBodySize = 0;

  EXPECT_NO_THROW(directive::setMaxBodySize("", maxBodySize));
  EXPECT_EQ(maxBodySize, 0);
  EXPECT_NO_THROW(directive::setMaxBodySize("0", maxBodySize));
  EXPECT_EQ(maxBodySize, std::numeric_limits<size_t>::max());
  EXPECT_NO_THROW(directive::setMaxBodySize("1024", maxBodySize));
  EXPECT_EQ(maxBodySize, 1024 * size::BYTE);
  EXPECT_NO_THROW(directive::setMaxBodySize("1024B", maxBodySize));
  EXPECT_EQ(maxBodySize, 1024 * size::BYTE);
  EXPECT_NO_THROW(directive::setMaxBodySize("1K", maxBodySize));
  EXPECT_EQ(maxBodySize, 1 * size::KILOBYTE);
  EXPECT_NO_THROW(directive::setMaxBodySize("1M", maxBodySize));
  EXPECT_EQ(maxBodySize, 1 * size::MEGABYTE);
  EXPECT_NO_THROW(directive::setMaxBodySize("1G", maxBodySize));
  EXPECT_EQ(maxBodySize, 1 * size::GIGABYTE);
  EXPECT_THROW(directive::setMaxBodySize("1T", maxBodySize), std::runtime_error);
  EXPECT_THROW(directive::setMaxBodySize("abc", maxBodySize), std::runtime_error);
}

// Test suite for addIndex
TEST(DirectiveTest, addIndex) {

  std::set<string> indexes;

  EXPECT_NO_THROW(directive::addIndex("", indexes));
  EXPECT_TRUE(indexes.empty());
  EXPECT_NO_THROW(directive::addIndex("index.html", indexes));
  EXPECT_EQ(indexes.size(), 1);
  EXPECT_TRUE(indexes.find("index.html") != indexes.end());
  EXPECT_NO_THROW(directive::addIndex("index.html index.php default.html", indexes));
  EXPECT_EQ(indexes.size(), 3);
  EXPECT_TRUE(indexes.find("index.html") != indexes.end());
  EXPECT_TRUE(indexes.find("index.php") != indexes.end());
  EXPECT_TRUE(indexes.find("default.html") != indexes.end());
}

TEST(DirectiveTest, setFastCgi) {

  std::string fastcgi;

  EXPECT_NO_THROW(directive::setFastCgi("", fastcgi));
  EXPECT_TRUE(fastcgi.empty());
  EXPECT_NO_THROW(directive::setFastCgi("/path/to/fastcgi", fastcgi));
  EXPECT_EQ(fastcgi, "/path/to/fastcgi");
  EXPECT_NO_THROW(directive::setFastCgi("/new/path", fastcgi));
  EXPECT_EQ(fastcgi, "/new/path");
}

TEST(DirectiveTest, addErrorPage) {

  std::map<string, string> errorPages;

  EXPECT_NO_THROW(directive::addErrorPage("", errorPages));
  EXPECT_TRUE(errorPages.empty());
  EXPECT_NO_THROW(directive::addErrorPage("404 /404.html", errorPages));
  EXPECT_EQ(errorPages["404"], "/404.html");
  EXPECT_NO_THROW(directive::addErrorPage("500 502 503 /50x.html", errorPages));
  EXPECT_EQ(errorPages["500"], "/50x.html");
  EXPECT_EQ(errorPages["502"], "/50x.html");
  EXPECT_EQ(errorPages["503"], "/50x.html");
  EXPECT_THROW(directive::addErrorPage("invalid", errorPages), std::runtime_error);
  EXPECT_THROW(directive::addErrorPage("600 /error.html", errorPages), std::runtime_error);
  EXPECT_THROW(directive::addErrorPage("abc /error.html", errorPages), std::runtime_error);
}

TEST(DirectiveTest, mergeErrorPages) {

  std::map<string, string> basePages;
  std::map<string, string> newPages;

  basePages["404"] = "/404.html";
  basePages["500"] = "/500.html";
  newPages["404"] = "/new404.html";
  newPages["503"] = "/503.html";

  EXPECT_NO_THROW(directive::mergeErrorPages(newPages, basePages));
  EXPECT_EQ(basePages["404"], "/404.html");  
  EXPECT_EQ(basePages["500"], "/500.html"); 
  EXPECT_EQ(basePages["503"], "/503.html"); 
}

TEST(DirectiveTest, validateHttpCode) {

  EXPECT_TRUE(directive::validateHttpCode("200"));
  EXPECT_TRUE(directive::validateHttpCode("404"));
  EXPECT_TRUE(directive::validateHttpCode("500"));
  EXPECT_FALSE(directive::validateHttpCode("99"));   // Too low
  EXPECT_FALSE(directive::validateHttpCode("600"));  // Too high
  EXPECT_FALSE(directive::validateHttpCode("abc"));  // Non-numeric
  EXPECT_FALSE(directive::validateHttpCode("4o4"));  // Contains letters
  EXPECT_FALSE(directive::validateHttpCode("-404")); // Negative
}

TEST(DirectiveTest, setReturn) {

  std::string code, uri;

  EXPECT_NO_THROW(directive::setReturn("", code, uri));
  EXPECT_TRUE(code.empty());
  EXPECT_TRUE(uri.empty());
  EXPECT_NO_THROW(directive::setReturn("404", code, uri));
  EXPECT_EQ(code, "404");
  EXPECT_TRUE(uri.empty());
  EXPECT_NO_THROW(directive::setReturn("301 /redirect", code, uri));
  EXPECT_EQ(code, "301");
  EXPECT_EQ(uri, "/redirect");
  EXPECT_THROW(directive::setReturn("invalid", code, uri), std::runtime_error);
  EXPECT_THROW(directive::setReturn("600 /error", code, uri), std::runtime_error);
  EXPECT_THROW(directive::setReturn("200 /ok extra", code, uri), std::runtime_error);
}

TEST(DirectiveTest, addServer) {

  std::vector<Server> servers;
  Server server1, server2;

  std::vector<std::string> listen1;
  listen1.push_back("127.0.0.1:8080");
  server1.setListen(listen1);

  std::vector<std::string> names1;
  names1.push_back("server1");
  server1.setNames(names1);

  EXPECT_NO_THROW(directive::addServer(server1, servers));
  EXPECT_EQ(servers.size(), 1);

  std::vector<std::string> listen2;
  listen2.push_back("192.168.1.1:8080");
  server2.setListen(listen2);

  std::vector<std::string> names2;
  names2.push_back("server2");
  server2.setNames(names2);

  EXPECT_NO_THROW(directive::addServer(server2, servers));
  EXPECT_EQ(servers.size(), 2);

  Server server3;
  server3.setListen(listen1);
  std::vector<std::string> names3;
  names3.push_back("server3");
  server3.setNames(names3);

  EXPECT_THROW(directive::addServer(server3, servers), runtime_error); 
}

TEST(DirectiveTest, setHttpDefaultValues) {

	Http *http = Http::getInstance();
	http->configure(getConfigPath());
    
    EXPECT_NO_THROW(directive::setHttpDefaultValues(*Http::getInstance()));
    EXPECT_FALSE(http->getIndexes().empty());
    EXPECT_EQ(http->getAutoIndexBitSet().to_ulong(), parser::AUTOINDEX_OFF);
    EXPECT_EQ(http->getRoot(), "/var/www/html");
}

TEST(DirectiveTest, setLocationDefaultValues) {
  Server server;
  Location location;

  server.setMaxBodySize("1024");
  server.setRoot("/var/www");
  std::set<std::string> indexes;
  indexes.insert("index.html");
  server.setIndexes(indexes);

  EXPECT_NO_THROW(directive::setLocationDefaultValues(server, location));
  EXPECT_EQ(location.getMaxBodySize(), 1024);
  EXPECT_EQ(location.getRoot(), "/var/www");
  EXPECT_EQ(location.getIndexes(), indexes);
  EXPECT_FALSE(location.getDenyMethods());
}
