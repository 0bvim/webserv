//
// Created by vinicius on 11/2/24.
//


#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>

#include "common.hpp"
#include "network/NetworkSetup.hpp"
#include "parser_config_file/CheckFile.hpp"
#include "parser_config_file/ConfigParser.hpp"

// Function to handle incoming requests (simplified version)
void handleClientRequest(int client_fd, const Location& location) {
    // Example of processing a CGI request (placeholder)
    if (location.cgi.enabled) {
        std::cout << "Executing CGI script: " << location.cgi.path << "\n";
        // Here, execute the CGI script using `execve` or similar.
    } else {
        std::cout << "Handling non-CGI request for client " << client_fd << "\n";
        // Handle static files, directory listing, etc.
    }

    // Respond to the client (simplified)
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";
    send(client_fd, response.c_str(), response.size(), 0);
    close(client_fd);
}

int main(int argc, char* argv[])
{
    std::string configuration_file;

    if (argc < 2)
        configuration_file = DEFAULT_CONFIG_FILE;
    else
        configuration_file = argv[1];

    CheckFile(configuration_file);

    try
    {
        ConfigParser parser;
        std::vector<Server> servers = parser.parse(configuration_file);

        //setup network
        NetworkSetup networkSetup;
        for (std::vector<Server>::const_iterator it = servers.begin(); it != servers.end(); ++it)
        {
            std::stringstream port;
            port << it->port;
            if (!networkSetup.setupServer(it->host, it->port))
                throw std::runtime_error("Failed to setup server on host: " + it->host + " port: " + port.str());
            std::cout << "Starting server on host: " << it->host << " port: " << it->port << "\n";
        }

        // main server loop
        while (42)
        {
            // accept new connections
            std::vector<int> serverSockets = networkSetup.getServerSockets();
            for (size_t i = 0; i < serverSockets.size(); i++)
            {
                int socket = serverSockets[i];
                int client_fd = NetworkSetup::acceptConnection(socket);
                if (client_fd < 0)
                    continue;

                std::cout << "Accepted new connection on socket " << client_fd << std::endl;

                // route the request to the appropriate location (simple)
                for (size_t j = 0; j < servers.size(); j++)
                {
                    const Server& server = servers[j];
                    for (size_t k = 0; k < server.locations.size(); k++)
                    {
                        const Location& location = server.locations[k];
                        // Here, you would normally parse the HTTP request and match against `location.path`
                        // For simplicity, we handle the first location
                        handleClientRequest(client_fd, location);
                        break;
                    }
                }
            }
        }
    }
    catch (const ConfigParseError& e)
    {
        std::cerr << "Error on line " << e.getLine() << ": " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

