#include "../include/Parser.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <cstring>


class testParser : public ::testing::Test
{
protected:
  Parser* parser;
  std::thread parserThread;

  virtual void SetUp()
  {
    parser = new Parser("generic string, need to test"); 
  }

  virtual void TearDown()
  {
    delete parser;
  }
};

