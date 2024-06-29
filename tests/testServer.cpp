#include "../include/Server.hpp"
#include <gtest/gtest.h>

/**
 * to create tests for your classes and function
 * you need to create a class that inherit from
 * 'public ::testing::Test'
 **/
class ServerTest : public ::testing::Test {
protected:
  Server *server;

  // to initialize with what you need to run your tests
  virtual void SetUp() { server = new Server(); }

  // to destroy, delete or do things that you should do when test ends.
  virtual void TearDown() { delete server; }
};

/**
 * TEST_F is the MACRO to you start the assertions first param is the class
 * that you make above second param is the name of you test. inside of brackets
 * you should insert your asserts
 *
 * this assert test the constructor and all functions
 * even the private functions, becaus the constructor use all.
 **/

TEST_F(ServerTest, Cosntructor) { ASSERT_NE(server, nullptr); }
