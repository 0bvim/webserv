#include <gtest/gtest.h>
#include "header.hpp"

TEST(HeaderTest, AcceptConstant) {
	EXPECT_EQ(header::ACCEPT, "Accept");
}

TEST(HeaderTest, ConnectionConstant) {
	EXPECT_EQ(header::CONNECTION, "Connection");
}

TEST(HeaderTest, ContentTypeConstant) {
	EXPECT_EQ(header::CONTENT_TYPE, "Content-Type");
}

TEST(HeaderTest, ContentLengthConstant) {
	EXPECT_EQ(header::CONTENT_LENGTH, "Content-Length");
}

TEST(HeaderTest, HostConstant) {
	EXPECT_EQ(header::HOST, "Host");
}

TEST(HeaderTest, LocationConstant) {
	EXPECT_EQ(header::LOCATION, "Location");
}

TEST(HeaderTest, KeepAliveConstant) {
	EXPECT_EQ(header::KEEP_ALIVE, "Keep-alive");
}

TEST(HeaderTest, RefererConstant) {
	EXPECT_EQ(header::REFERER, "Referer");
}

TEST(HeaderTest, ServerConstant) {
	EXPECT_EQ(header::SERVER, "Server");
}

TEST(HeaderTest, UserAgentConstant) {
	EXPECT_EQ(header::USER_AGENT, "User-Agent");
}

TEST(HeaderTest, TransferEncondingConstant) {
	EXPECT_EQ(header::TRANSFER_ENCONDING, "Transfer-Encoding");
}
