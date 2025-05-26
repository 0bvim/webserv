#include "gtest/gtest.h"
#include "Common.hpp"
#include "request.hpp"
#include "Connection.hpp"
#include "response.hpp"
#include "step.hpp"
#include "../utils/test_helpers.hpp"

using namespace std;

Mime *mimes = NULL;

TEST(RequestTest, ParseRequest_ValidRequest) {
	mimes = Mime::getInstance();
	mimes->configure(getMimesPath());
	Connection connection(5, "127.0.0.1");
	string line = "GET /index.html HTTP/1.1\r";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getMethod(), "GET");
	EXPECT_EQ(connection.getTarget(), "/index.html");
	EXPECT_EQ(connection.getProtocol(), "HTTP/1.1");
}

TEST(RequestTest, ParseRequest_ValidRequestWithQueryString) {
	mimes = Mime::getInstance();
	mimes->configure(getMimesPath());
	Connection connection(5, "127.0.0.1");
	string line = "GET /index.html?name=changes HTTP/1.1\r";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getMethod(), "GET");
	EXPECT_EQ(connection.getTarget(), "/index.html?name=changes");
	EXPECT_EQ(connection.getProtocol(), "HTTP/1.1");
}

TEST(RequestTest, ParseRequest_InvalidSpacing) {
	mimes = Mime::getInstance();
	mimes->configure(getMimesPath());
	Connection connection(5, "127.0.0.1");
	string line = "        GET    /index.html     HTTP/1.1   \r";
	request::parseRequest(&connection, line);
	EXPECT_NE(connection.getMethod(), "GET") << "Method should not be GET";
	EXPECT_NE(connection.getTarget(), "/index.html") << "Path should not be /index.html";
	EXPECT_EQ(connection.getProtocol(), "HTTP/1.1") << "Protocol should not be HTTP/1.1";
	EXPECT_EQ(connection.getCode(), "400") << "Code should be 200";
}

TEST(RequestTest, ParseRequest_InvalidSpacingDuplicate) {
	mimes = Mime::getInstance();
	mimes->configure(getMimesPath());
	Connection connection(5, "127.0.0.1");
	string line = "        GET    /index.html     HTTP/1.1   \r";
	request::parseRequest(&connection, line);
	EXPECT_NE(connection.getMethod(), "GET") << "Method should not be GET";
	EXPECT_NE(connection.getTarget(), "/index.html") << "Path should not be /index.html";
	EXPECT_EQ(connection.getProtocol(), "HTTP/1.1") << "Protocol should not be HTTP/1.1";
	EXPECT_EQ(connection.getCode(), "400") << "Code should be 200";
}

TEST(RequestTest, ParseRequest_MissingPath) {
	mimes = Mime::getInstance();
	mimes->configure(getMimesPath());
	Connection connection(5, "127.0.0.1");
	string line = "GET HTTP/1.1\r";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getMethod(), "") << "Method should be GET";
	EXPECT_EQ(connection.getTarget(), "") << "Path should be empty";
	EXPECT_EQ(connection.getProtocol(), "HTTP/1.1") << "Protocol should be HTTP/1.1";
	EXPECT_EQ(connection.getCode(), "400") << "Code should be 400";
}

TEST(RequestTest, ParseRequest_InvalidMethod) {
	mimes = Mime::getInstance();
	mimes->configure(getMimesPath());
	Connection connection(5, "127.0.0.1");
	string line = "PATCH /index.html HTTP/1.1\r";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getCode(), "405") << "Code should be 405";
}

TEST(RequestTest, ParseRequest_InvalidMethodDuplicate) {
	mimes = Mime::getInstance();
	mimes->configure(getMimesPath());
	Connection connection(5, "127.0.0.1");
	string line = "PATCH /index.html HTTP/1.1\r";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getCode(), "405") << "Code should be 405";
}

TEST(RequestTest, ParseRequest_InvalidProtocol) {
	mimes = Mime::getInstance();
	mimes->configure(getMimesPath());
	Connection connection(5, "127.0.0.1");
	string line = "GET /index.html HTTP/1.2\r";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getCode(), "505") << "Code should be 400";
}

TEST(RequestTest, ParseRequest_HeadersParsed) {
	mimes = Mime::getInstance();
	mimes->configure(getMimesPath());
	Connection connection(5, "127.0.0.1");
	string line = "GET /index.html HTTP/1.1\r";
	request::parseRequest(&connection, line);
	line = "\r";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getStep(), step::RESPONSE);
}

TEST(RequestTest, ParseRequest_HeadersNotParsed) {
	mimes = Mime::getInstance();
	mimes->configure(getMimesPath());
	Connection connection(5, "127.0.0.1");
	string line = "GET /index.html HTTP/1.1\r";
	request::parseRequest(&connection, line);
	EXPECT_EQ(connection.getStep(), step::STARTLINE);
	EXPECT_NE(connection.getStep(), step::HEADERS);
}
