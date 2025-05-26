#include "status.hpp"
#include <gtest/gtest.h>

TEST(StatusTest, OK) {
	EXPECT_EQ(status::OK, "OK");
}

TEST(StatusTest, Created) {
	EXPECT_EQ(status::CREATED, "Created");
}

TEST(StatusTest, Accepted) {
	EXPECT_EQ(status::ACCEPTED, "Accepted");
}

TEST(StatusTest, NoContent) {
	EXPECT_EQ(status::NO_CONTENT, "No Content");
}

TEST(StatusTest, MovedPermanently) {
	EXPECT_EQ(status::MOVED_PERMANENTLY, "Moved Permanently");
}

TEST(StatusTest, BadRequest) {
	EXPECT_EQ(status::BAD_REQUEST, "Bad Request");
}

TEST(StatusTest, Unauthorized) {
	EXPECT_EQ(status::UNAUTHORIZED, "Unauthorized");
}

TEST(StatusTest, FORBIDDEN) {
	EXPECT_EQ(status::FORBIDDEN, "Forbidden");
}

TEST(StatusTest, NotFound) {
	EXPECT_EQ(status::NOT_FOUND, "Not Found");
}

TEST(StatusTest, NotAllowed) {
	EXPECT_EQ(status::NOT_ALLOWED, "Method Not Allowed");
}

TEST(StatusTest, LengthRequired) {
	EXPECT_EQ(status::LENGTH_REQUIRED, "Length Required");
}

TEST(StatusTest, PayloadTooLarge) {
	EXPECT_EQ(status::PAYLOAD_TOO_LARGE, "Payload Too Large");
}

TEST(StatusTest, URITooLong) {
	EXPECT_EQ(status::URI_TOO_LONG, "URI Too Long");
}

TEST(StatusTest, UnsupportedMediaType) {
	EXPECT_EQ(status::UNSUPPORTED_MEDIA_TYPE, "Unsupported Media Type");
}

TEST(StatusTest, UnprocessableContent) {
	EXPECT_EQ(status::UNPROCESSABLE_CONTENT, "Unprocessable Content");
}

TEST(StatusTest, InternalServerError) {
	EXPECT_EQ(status::INTERNAL_SERVER_ERROR, "Internal Server Error");
}

TEST(StatusTest, NotImplemented) {
	EXPECT_EQ(status::NOT_IMPLEMENTED, "Not Implemented");
}

TEST(StatusTest, GatewayTimeout) {
	EXPECT_EQ(status::GATEWAY_TIMEOUT, "Gateway Timeout");
}

TEST(StatusTest, HttpVersionNotSupported) {
	EXPECT_EQ(status::HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported");
}
