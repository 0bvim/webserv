#include "../include/Client.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>

class MockClient : public Client {
public:
  MOCK_METHOD(void, _createClientSocket, (), (override));
  MOCK_METHOD(void, connectToServer, (const int serverSocket), (override));
};

TEST(ClientTest, ConstructorDestructor) {
  MockClient client;
  // Verify constructor and destructor behavior
}

TEST(ClientTest, ConnectToServerSuccess) {
  MockClient client;

  int serverSocket = 5; // Mock server socket
  struct sockaddr_in address;
  socklen_t addressSize = sizeof(address);

  EXPECT_CALL(client, connectToServer(serverSocket)).Times(1);

  client.connectToServer(serverSocket);
}

TEST(ClientTest, ConnectToServerFailure) {
  MockClient client;

  int serverSocket = -1; // Invalid server socket

  EXPECT_THROW(client.connectToServer(serverSocket), std::runtime_error);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
