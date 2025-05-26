#include "gtest/gtest.h"
#include "code.hpp"
#include "response.hpp"
#include "gmock/gmock.h"
#include "Connection.hpp"
#include "status.hpp"
#include <type_traits>

TEST(CodeTest, OK) {
	EXPECT_EQ(code::OK, "200");
}

TEST(CodeTest, Created) {
	EXPECT_EQ(code::CREATED, "201");
}

TEST(CodeTest, Accepted) {
	EXPECT_EQ(code::ACCEPTED, "202");
}

TEST(CodeTest, NoContent) {
	EXPECT_EQ(code::NO_CONTENT, "204");
}

TEST(CodeTest, MovedPermanently) {
	EXPECT_EQ(code::MOVED_PERMANENTLY, "301");
}

TEST(CodeTest, BadRequest) {
	EXPECT_EQ(code::BAD_REQUEST, "400");
}

TEST(CodeTest, Unauthorized) {
	EXPECT_EQ(code::UNAUTHORIZED, "401");
}

TEST(CodeTest, FORBIDDEN) {
	EXPECT_EQ(code::FORBIDDEN, "403");
}

TEST(CodeTest, NotFound) {
	EXPECT_EQ(code::NOT_FOUND, "404");
}

TEST(CodeTest, NotAllowed) {
	EXPECT_EQ(code::NOT_ALLOWED, "405");
}

TEST(CodeTest, LengthRequired) {
	EXPECT_EQ(code::LENGTH_REQUIRED, "411");
}

TEST(CodeTest, PayloadTooLarge) {
	EXPECT_EQ(code::PAYLOAD_TOO_LARGE, "413");
}

TEST(CodeTest, UriTooLong) {
	EXPECT_EQ(code::URI_TOO_LONG, "414");
}

TEST(CodeTest, UnsupportedMediaType) {
	EXPECT_EQ(code::UNSUPPORTED_MEDIA_TYPE, "415");
}

TEST(CodeTest, UnprocessableContent) {
	EXPECT_EQ(code::UNPROCESSABLE_CONTENT, "422");
}

TEST(CodeTest, InternalServerError) {
	EXPECT_EQ(code::INTERNAL_SERVER_ERROR, "500");
}

TEST(CodeTest, NotImplemented) {
	EXPECT_EQ(code::NOT_IMPLEMENTED, "501");
}

TEST(CodeTest, GatewayTimeout) {
	EXPECT_EQ(code::GATEWAY_TIMEOUT, "504");
}

TEST(CodeTest, HttpVersionNotSupported) {
	EXPECT_EQ(code::HTTP_VERSION_NOT_SUPPORTED, "505");
}
