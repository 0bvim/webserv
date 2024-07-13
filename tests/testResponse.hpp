#include <gtest/gtest.h>
#include "../include/Response.hpp"
#include "../include/Request.hpp"

class ResponseTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char * requestStr = "GET /index.html HTTP/1.1\r\nHost: example.com\r\n\r\n";
        request = new Request(requestStr);
        response = new Response(*request);
    }

    void TearDown() override {
        delete response;
        delete request;
    }

    Request *request;
    Response *response;
};

TEST_F(ResponseTest, GenerateResponse) {
    std::string responseStr = response->getResponse();
    EXPECT_NE(responseStr.find("HTTP/1.1 200 OK"), std::string::npos);
    EXPECT_NE(responseStr.find("Content-Type: text/html"), std::string::npos);
    EXPECT_NE(responseStr.find("Content-Length"), std::string::npos);
    EXPECT_NE(responseStr.find("<html>"), std::string::npos);  // Assuming your index.html has <html> tag
}
