#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <exception>
#include "../network/NetworkSetup.hpp"
#include "../parser_config_file/ConfigTypes.hpp"

class Server_class
{
private:
    NetworkSetup _network;
    bool _is_running;
    Server_class(const Server_class& other);
    Server_class& operator=(const Server_class& other);

public:
    std::string host;
    int port;
    std::vector<std::string> server_names;
    size_t client_max_size;
    std::map<int, std::string> error_pages;
    std::vector<Location> locations;

    Server_class();
    ~Server_class();

    bool start();
    void stop();
    bool isRunning() const;
    int acceptClient();
    const std::vector<int>& getServerSockets() const;

};

#endif // SERVER_HPP