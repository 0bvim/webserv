#include <gtest/gtest.h>
#include "Request.hpp"

TEST(RequestTest, ParseSimpleGetRequest) {
    const char * requestStr = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n";
    Request request(requestStr);
    t_request parsedRequest = request.getRequest();

    EXPECT_EQ(parsedRequest.method, "GET");
    EXPECT_EQ(parsedRequest.uri, "/index.html");
    ASSERT_EQ(parsedRequest.headers.size(), 1);
    EXPECT_EQ(parsedRequest.headers["Host"], "example.com");
}

TEST(RequestTest, ParseRequestWithMultipleHeaders) {
    const char * requestStr = "POST /submit HTTP/1.1\r\nHost: example.com\r\nContent-Length: 27\r\n\r\n";
    Request request(requestStr);
    t_request parsedRequest = request.getRequest();

    EXPECT_EQ(parsedRequest.method, "POST");
    EXPECT_EQ(parsedRequest.uri, "/submit");
    ASSERT_EQ(parsedRequest.headers.size(), 2);
    EXPECT_EQ(parsedRequest.headers["Host"], "example.com");
    EXPECT_EQ(parsedRequest.headers["Content-Length"], "27");
}
