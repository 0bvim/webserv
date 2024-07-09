#include "../include/Server.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class ServerTest : public ::testing::Test {
protected:
    Server* server;
    std::thread serverThread;

    virtual void SetUp() {
        server = new Server("127.0.0.1", 8080);
        serverThread = std::thread([this]() {
            server->run();
        });
        // Give the server a moment to start up
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    virtual void TearDown() {
        // Stop the server thread
        if (serverThread.joinable()) {
            serverThread.detach();
        }
        delete server;
    }
};

TEST_F(ServerTest, ServerStartsSuccessfully) {
    // Test if the server starts and listens successfully
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(sock, -1);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    int result = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    ASSERT_NE(result, -1);

    close(sock);
}

TEST_F(ServerTest, ServerHandlesClientConnection) {
    // Test if the server can handle a client connection
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(sock, -1);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8080);

    int result = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    ASSERT_NE(result, -1);

    const char* message = "Hello Server";
    send(sock, message, strlen(message), 0);

    char buffer[1024];
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    ASSERT_GT(bytes_received, 0);

    buffer[bytes_received] = '\0';
    std::string response(buffer);
    ASSERT_EQ(response, "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!");

    close(sock);
}

TEST_F(ServerTest, HandlesMultipleClientConnections) {
    const int num_clients = 5;
    int client_socks[num_clients];

    for (int i = 0; i < num_clients; ++i) {
        client_socks[i] = socket(AF_INET, SOCK_STREAM, 0);
        ASSERT_NE(client_socks[i], -1);

        sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(8080);

        int result = connect(client_socks[i], (struct sockaddr*)&server_addr, sizeof(server_addr));
        ASSERT_NE(result, -1);
    }

    for (int i = 0; i < num_clients; ++i) {
        const char* message = "Hello Server";
        send(client_socks[i], message, strlen(message), 0);

        char buffer[1024];
        int bytes_received = recv(client_socks[i], buffer, sizeof(buffer) - 1, 0);
        ASSERT_GT(bytes_received, 0);

        buffer[bytes_received] = '\0';
        std::string response(buffer);
        ASSERT_EQ(response, "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!");

        close(client_socks[i]);
    }
}
