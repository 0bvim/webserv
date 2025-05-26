#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "response.hpp"
#include "code.hpp"
#include "status.hpp"
#include "Connection.hpp"
#include <string>


TEST(ResponseBuilderSimpleTest, MapIsAccessible) {

    response::responses["test"] = "test";
    
    EXPECT_EQ(response::responses["test"], "test");
}

TEST(ResponseBuilderSimpleTest, MapCanBeModified) {

    response::responses.clear();
    response::responses[code::OK] = status::OK;
    
    EXPECT_EQ(response::responses[code::OK], status::OK);
}

TEST(ResponseBuilderManualTest, MapCanBeInitializedCorrectly) {
    
    response::responses.clear();
    
    response::responses[code::OK] = status::OK;
    response::responses[code::CREATED] = status::CREATED;
    response::responses[code::ACCEPTED] = status::ACCEPTED;
    response::responses[code::NO_CONTENT] = status::NO_CONTENT;
    response::responses[code::MOVED_PERMANENTLY] = status::MOVED_PERMANENTLY;
    response::responses[code::BAD_REQUEST] = status::BAD_REQUEST;
    response::responses[code::UNAUTHORIZED] = status::UNAUTHORIZED;
    response::responses[code::FORBIDDEN] = status::FORBIDDEN;
    response::responses[code::NOT_FOUND] = status::NOT_FOUND;
    response::responses[code::NOT_ALLOWED] = status::NOT_ALLOWED;
    response::responses[code::CONFLICT] = status::CONFLICT;
    response::responses[code::LENGTH_REQUIRED] = status::LENGTH_REQUIRED;
    response::responses[code::PAYLOAD_TOO_LARGE] = status::PAYLOAD_TOO_LARGE;
    response::responses[code::URI_TOO_LONG] = status::URI_TOO_LONG;
    response::responses[code::UNSUPPORTED_MEDIA_TYPE] = status::UNSUPPORTED_MEDIA_TYPE;
    response::responses[code::UNPROCESSABLE_CONTENT] = status::UNPROCESSABLE_CONTENT;
    response::responses[code::INTERNAL_SERVER_ERROR] = status::INTERNAL_SERVER_ERROR;
    response::responses[code::NOT_IMPLEMENTED] = status::NOT_IMPLEMENTED;
    response::responses[code::BAD_GATEWAY] = status::BAD_GATEWAY;
    response::responses[code::GATEWAY_TIMEOUT] = status::GATEWAY_TIMEOUT;
    response::responses[code::HTTP_VERSION_NOT_SUPPORTED] = status::HTTP_VERSION_NOT_SUPPORTED;
    
    EXPECT_FALSE(response::responses.empty());
    EXPECT_EQ(response::responses[code::OK], status::OK);
    EXPECT_EQ(response::responses[code::NOT_FOUND], status::NOT_FOUND);
    EXPECT_EQ(response::responses[code::BAD_REQUEST], status::BAD_REQUEST);
    EXPECT_EQ(response::responses[code::INTERNAL_SERVER_ERROR], status::INTERNAL_SERVER_ERROR);
}

TEST(ResponseBuilderManualTest, MapHasAllExpectedCodes) {

    response::responses.clear();
    response::responses[code::OK] = status::OK;
    response::responses[code::CREATED] = status::CREATED;
    response::responses[code::ACCEPTED] = status::ACCEPTED;
    response::responses[code::NO_CONTENT] = status::NO_CONTENT;
    response::responses[code::MOVED_PERMANENTLY] = status::MOVED_PERMANENTLY;
    response::responses[code::BAD_REQUEST] = status::BAD_REQUEST;
    response::responses[code::UNAUTHORIZED] = status::UNAUTHORIZED;
    response::responses[code::FORBIDDEN] = status::FORBIDDEN;
    response::responses[code::NOT_FOUND] = status::NOT_FOUND;
    response::responses[code::NOT_ALLOWED] = status::NOT_ALLOWED;
    response::responses[code::CONFLICT] = status::CONFLICT;
    response::responses[code::LENGTH_REQUIRED] = status::LENGTH_REQUIRED;
    response::responses[code::PAYLOAD_TOO_LARGE] = status::PAYLOAD_TOO_LARGE;
    response::responses[code::URI_TOO_LONG] = status::URI_TOO_LONG;
    response::responses[code::UNSUPPORTED_MEDIA_TYPE] = status::UNSUPPORTED_MEDIA_TYPE;
    response::responses[code::UNPROCESSABLE_CONTENT] = status::UNPROCESSABLE_CONTENT;
    response::responses[code::INTERNAL_SERVER_ERROR] = status::INTERNAL_SERVER_ERROR;
    response::responses[code::NOT_IMPLEMENTED] = status::NOT_IMPLEMENTED;
    response::responses[code::BAD_GATEWAY] = status::BAD_GATEWAY;
    response::responses[code::GATEWAY_TIMEOUT] = status::GATEWAY_TIMEOUT;
    response::responses[code::HTTP_VERSION_NOT_SUPPORTED] = status::HTTP_VERSION_NOT_SUPPORTED;
    
    EXPECT_TRUE(response::responses.find(code::OK) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::CREATED) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::ACCEPTED) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::NO_CONTENT) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::MOVED_PERMANENTLY) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::BAD_REQUEST) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::UNAUTHORIZED) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::FORBIDDEN) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::NOT_FOUND) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::NOT_ALLOWED) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::CONFLICT) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::LENGTH_REQUIRED) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::PAYLOAD_TOO_LARGE) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::URI_TOO_LONG) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::UNSUPPORTED_MEDIA_TYPE) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::UNPROCESSABLE_CONTENT) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::INTERNAL_SERVER_ERROR) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::NOT_IMPLEMENTED) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::BAD_GATEWAY) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::GATEWAY_TIMEOUT) != response::responses.end());
    EXPECT_TRUE(response::responses.find(code::HTTP_VERSION_NOT_SUPPORTED) != response::responses.end());
} 