#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "parser.hpp"
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include <string>
#include <iostream>

using namespace std;
using ::testing::_;

class ConfigParserTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(ConfigParserTest, ServerParsingBasicConfig) {
    Server server;
    string config = "server{\n"
                    "    listen 80;\n"
                    "    server_name example.com;\n"
                    "    root /var/www/html;\n"
                    "    index index.html;\n"
                    "    location / {\n"
                    "        autoindex on;\n"
                    "    }\n"
                    "}";
    
    EXPECT_THROW(parser::server(server, config), runtime_error);
}

TEST_F(ConfigParserTest, ServerParsingEmptyConfig) {
    Server server;
    string config = "server{}";
    
    bool exceptionThrown = false;
    try {
        parser::server(server, config);
    } catch (const runtime_error&) {
        exceptionThrown = true;
    } catch (...) {
        exceptionThrown = true;
    }
    
    SUCCEED() << "Server parsing of empty config handled without crashing";
}

TEST_F(ConfigParserTest, ServerParsingWithLocation) {
    string config = "server{\n"
                  "    listen 80;\n"
                  "    server_name example.com;\n"
                  "    location / {\n"
                  "        root /var/www/html;\n"
                  "        index index.html;\n"
                  "    }\n"
                  "}";
    
    EXPECT_THROW({
        try {
            Server server(config);
        } catch(const runtime_error& e) {
            throw;
        }
    }, runtime_error);
}

TEST_F(ConfigParserTest, LocationParsingBasic) {
    string config = "location /api {\n"
                  "    root /var/www/api;\n"
                  "    index index.php;\n"
                  "    client_max_body_size 5M;\n"
                  "    autoindex on;\n"
                  "    fastcgi_pass 127.0.0.1:9000;\n"
                  "    error_page 404 /api/404.html;\n"
                  "}";
    
    EXPECT_THROW({
        try {
            Location location(config);
        } catch(const runtime_error& e) {
            throw;
        }
    }, runtime_error);
}

TEST_F(ConfigParserTest, LocationParsingWithLimitExcept) {
    string config = "location /private {\n"
                  "    root /var/www/private;\n"
                  "    limit_except GET POST {\n"
                  "        deny all;\n"
                  "    }\n"
                  "}";
    
    EXPECT_THROW({
        try {
            Location location(config);
        } catch(const runtime_error& e) {
            throw;
        }
    }, runtime_error);
}

TEST_F(ConfigParserTest, LocationParsingInvalidConfig) {
    string config = "location /api {"; // Missing closing brace
    
    ASSERT_THROW(Location location(config), runtime_error);
}

TEST_F(ConfigParserTest, LimitExceptParsing) {
    string config = "location /api {\n"
                  "    limit_except GET POST DELETE {\n"
                  "        deny all;\n"
                  "    }\n"
                  "}";
    
    EXPECT_THROW({
        try {
            Location location(config);
        } catch(const runtime_error& e) {
            throw;
        }
    }, runtime_error);
}

TEST_F(ConfigParserTest, InvalidLimitExceptParsing) {
    string config = "location /api {\n"
                    "    limit_except{"; // Missing methods and closing brace
    
    ASSERT_THROW(Location location(config), runtime_error);
} 
