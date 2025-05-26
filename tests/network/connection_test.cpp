#include <gtest/gtest.h>
#include "Common.hpp"
#include "Connection.hpp"
#include "header.hpp"
#include "Server.hpp"
#include <string>
#include <map>

using namespace std;

Http *http = NULL;

class ConnectionDestructorTest : public ::testing::Test
{
protected:
	std::vector<Connection *> test_connections;
	std::vector<string> methods = {"GET", "POST", "DELETE"};

	void SetUp() override
	{
		test_connections.push_back(new Connection(1234, "127.0.0.1"));
		test_connections.push_back(new Connection(5678, "192.168.1.1"));

		test_connections[0]->setMethod(methods.at(0));
		test_connections[0]->setTarget("/index.html");
		test_connections[0]->setProtocol("HTTP/1.1");

		test_connections[1]->setMethod(methods.at(1));
		test_connections[1]->setTarget("/api/data");
		test_connections[1]->setProtocol("HTTP/1.1");
	}

	void TearDown() override
	{
		for (Connection *conn : test_connections)
		{
			delete conn;
		}
		test_connections.clear();
	}
};

TEST_F(ConnectionDestructorTest, BasicDestructor)
{
	{
		Connection conn(1234, "127.0.0.1");
	}
	SUCCEED();
}

TEST_F(ConnectionDestructorTest, PopulatedConnectionDestructor)
{
	{
		Connection conn(1234, "127.0.0.1");

		conn.setMethod(methods.at(0));
		conn.setTarget("/index.html");
		conn.setProtocol("HTTP/1.1");
		conn.setHost("example.com");
		conn.setBody("Test body");

		std::map<std::string, std::string> headers;
		headers["Content-Type"] = "text/html";
		headers["Content-Length"] = "9";
		conn.setHeaders(headers);
	}
	SUCCEED();
}

TEST_F(ConnectionDestructorTest, CopyConstructedDestructor)
{
	{
		Connection conn1(1234, "127.0.0.1");
		conn1.setMethod(methods.at(0));
		conn1.setTarget("/index.html");

		Connection conn2(conn1);
	}
	SUCCEED();
}

TEST_F(ConnectionDestructorTest, AssignmentDestructor)
{
	{
		Connection conn1(1234, "127.0.0.1");
		conn1.setMethod(methods.at(0));

		Connection conn2(5678, "192.168.1.1");
		conn2 = conn1; // Assignment
	}
	SUCCEED();
}

TEST_F(ConnectionDestructorTest, DestructorAfterReset)
{
	{
		Connection conn(1234, "127.0.0.1");

		conn.setMethod(methods.at(0));
		conn.setTarget("/index.html");
		conn.setProtocol("HTTP/1.1");
		conn.resetConnection();
	}
	SUCCEED();
}

TEST_F(ConnectionDestructorTest, MultipleConnectionsDestruction)
{
	{
		std::vector<Connection *> connections;

		for (int i = 0; i < 5; i++)
		{
			connections.push_back(new Connection(1000 + i, "127.0.0.1"));
		}

		for (Connection *conn : connections)
		{
			delete conn;
		}
	}
	SUCCEED();
}

TEST_F(ConnectionDestructorTest, UseCommonConnection)
{
	ASSERT_EQ(test_connections[0]->getMethod(), "GET");
	ASSERT_EQ(test_connections[1]->getMethod(), "POST");
}
