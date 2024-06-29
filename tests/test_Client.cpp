// tests/test_Client.cpp

#include <gtest/gtest.h>
#include "../include/Client.hpp"

// Mocking socket functions for testing
#include <sys/socket.h>
#include <unistd.h>

class ClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any common initialization here
    }

    void TearDown() override {
        // Clean up any common cleanup here
    }
};

TEST_F(ClientTest, ConstructorDestructor) {
    Client client;
    // If the destructor runs without exceptions, the test will pass
}

TEST_F(ClientTest, ConnectToServerSuccess) {
    Client client;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(serverSocket, -1) << "Failed to create server socket";

    // Bind and listen to the socket to simulate a server
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(8080);

    int bindResult = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    ASSERT_NE(bindResult, -1) << "Failed to bind server socket";

    int listenResult = listen(serverSocket, 1);
    ASSERT_NE(listenResult, -1) << "Failed to listen on server socket";

    // Create a client socket to connect to the server
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(clientSocket, -1) << "Failed to create client socket";

    // Connect to the server
    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    clientAddress.sin_port = htons(8080);

    int connectResult = connect(clientSocket, (struct sockaddr*)&clientAddress, sizeof(clientAddress));
    ASSERT_NE(connectResult, -1) << "Failed to connect to the server";

    // Use the client's connectToServer method to accept the connection
    client.connectToServer(serverSocket);

    close(serverSocket);
    close(clientSocket);
}

TEST_F(ClientTest, ConnectToServerFailure) {
    Client client;
    int invalidServerSocket = -1; // Invalid server socket

    EXPECT_THROW(client.connectToServer(invalidServerSocket), std::runtime_error);
}

