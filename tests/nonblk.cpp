#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

// Function to create and configure the server socket
int create_server_socket(int port)
{
	int server_fd;
	struct sockaddr_in address;

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	// Bind the socket to the network address and port
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// Start listening on the socket
	if (listen(server_fd, 10) < 0)
	{
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	return server_fd;
}

// Function to set a file descriptor to non-blocking mode using ioctl
void set_socket_non_blocking(int fd)
{
	int flags = 1;
	if (ioctl(fd, FIONBIO, &flags) < 0)
	{
		perror("ioctl failed");
		exit(EXIT_FAILURE);
	}
}

// Function to handle incoming connections
void handle_connections(int server_fd)
{
	int epoll_fd = epoll_create1(0);
	if (epoll_fd == -1)
	{
		perror("epoll_create1 failed");
		exit(EXIT_FAILURE);
	}

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = server_fd;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1)
	{
		perror("epoll_ctl failed");
		exit(EXIT_FAILURE);
	}

	struct epoll_event events[10];
	while (true)
	{
		int n = epoll_wait(epoll_fd, events, 10, -1);
		for (int i = 0; i < n; i++)
		{
			if (events[i].data.fd == server_fd)
			{
				struct sockaddr_in client_address;
				socklen_t client_len = sizeof(client_address);
				int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
				if (client_fd < 0)
				{
					perror("accept failed");
					continue;
				}

				// Set the client socket to non-blocking
				// fcntl(client_fd, F_SETFL, O_NONBLOCK);
				set_socket_non_blocking(client_fd);

				event.events = EPOLLIN | EPOLLET;
				event.data.fd = client_fd;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
				{
					perror("epoll_ctl failed");
					close(client_fd);
				}
			}
			else
			{
				char buffer[1024] = {0};
				int client_fd = events[i].data.fd;
				int bytes_read = read(client_fd, buffer, sizeof(buffer));

				if (bytes_read > 0)
				{
					std::cout << "Received request:\n"
							  << buffer << std::endl;

					// Check if it's a GET request
					if (strncmp(buffer, "GET", 3) == 0)
					{
						const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
						send(client_fd, response, strlen(response), 0);
					}
				}

				close(client_fd);
			}
		}
	}
	close(epoll_fd);
}

// Main function to run the server
int main()
{
	int server_fd = create_server_socket(8042); // Use the desired port
	std::cout << "Server is listening on port 8042" << std::endl;
	handle_connections(server_fd);

	close(server_fd);
	return 0;
}
