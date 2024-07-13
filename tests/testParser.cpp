// #include "../include/Parser.hpp"
// #include <gtest/gtest.h>
// #include <thread>
// #include <chrono>
// #include <unistd.h>
// #include <cstring>


// class testParser : public ::testing::Test
// {
// protected:
//   Parser* parser;
//   std::thread parserThread;

//   virtual void SetUp() {}

//   virtual void TearDown() {}
// };

// TEST(ParserTest, EmptyInput)
// {
//   Parser parser("");
//   t_request request = parser.getRequest();
//   EXPECT_EQ(request.method, "");
//   EXPECT_EQ(request.uri, "");
//   EXPECT_EQ(request.headers.size(), 0);
// }

// TEST(ParserTest, SingleLineRequest)
// {
//   Parser parser("GET /index.html");
//   t_request request = parser.getRequest();
//   EXPECT_EQ(request.method, "GET");
//   EXPECT_EQ(request.uri, "/index.html");
//   EXPECT_EQ(request.headers.size(), 0);
// }

// TEST(ParserTest, RequestWithHeaders)
// {
//   const char* requestStr = "GET /about HTTP/1.1\r\n"
//                            "Host: example.com\r\n"
//                            "Connection: keep-alive\r\n\r\n";
//   Parser parser(requestStr);
//   t_request request = parser.getRequest();
//   EXPECT_EQ(request.method, "GET");
//   EXPECT_EQ(request.uri, "/about");
//   EXPECT_EQ(request.headers.size(), 2);
//   EXPECT_EQ(request.headers["Host"], "example.com");
//   EXPECT_EQ(request.headers["Connection"], "keep-alive");
// }

// TEST(ParserTest, EmptyLineAfterHeaders)
// {
//   const char* requestStr = "GET /contact\r\n"
//                            "Content-Length: 100\r\n"
//                            "\r\n";
//   Parser parser(requestStr);
//   t_request request = parser.getRequest();
//   EXPECT_EQ(request.method, "GET");
//   EXPECT_EQ(request.uri, "/contact");
//   EXPECT_EQ(request.headers.size(), 1);
//   EXPECT_EQ(request.headers["Content-Length"], "100");
// }
