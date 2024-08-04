#include <gtest/gtest.h>
#include "../include/Config.hpp"

TEST(ConfigTest, ConstructorSetsFilePath)
{
	Config config("test_default.conf");
	EXPECT_EQ(config.getServerAddress(), "127.0.0.1"); // Example expectation based on your config file
}

TEST(ConfigTest, ParseConfigFilePopulatesServers)
{
	Config config("test_default.conf");
	EXPECT_GT(config.getServers().size(), 0); // Expect at least one server configuration
}
