#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

struct Location
{
	std::string path;
	std::map<std::string, std::string> directives;
};

struct Server
{
	std::string server_name;
	std::string listen;
	std::map<std::string, std::string> directives;
	std::vector<Location> locations;
};

struct NginxConfig
{
	std::vector<Server> servers;
};

class NginxParser
{
public:
	NginxConfig parseConfig(const std::string &filename)
	{
		std::ifstream file(filename.c_str());
		NginxConfig config;
		std::string line;
		Server currentServer;
		Location currentLocation;
		bool inServerBlock = false;
		bool inLocationBlock = false;

		while (std::getline(file, line))
		{
			line = trim(line);

			if (line.empty() || line[0] == '#')
				continue; // Skip comments
			if (line == "server {")
			{
				inServerBlock = true;
				currentServer = Server();
			}
			else if (line == "}")
			{
				if (inLocationBlock)
				{
					currentServer.locations.push_back(currentLocation);
					inLocationBlock = false;
				}
				else if (inServerBlock)
				{
					config.servers.push_back(currentServer);
					inServerBlock = false;
				}
			}
			else if (line.find("location") == 0)
			{
				inLocationBlock = true;
				currentLocation = Location();
				currentLocation.path = line.substr(9, line.find("{") - 9);
				currentLocation.path = trim(currentLocation.path);
			}
			else if (inLocationBlock)
			{
				size_t pos = line.find(' ');
				std::string directive = line.substr(0, pos);
				std::string value = trim(line.substr(pos + 1));
				currentLocation.directives[directive] = value;
			}
			else if (inServerBlock)
			{
				size_t pos = line.find(' ');
				std::string directive = line.substr(0, pos);
				std::string value = trim(line.substr(pos + 1));
				if (directive == "server_name")
					currentServer.server_name = value;
				else if (directive == "listen")
					currentServer.listen = value;
				else
					currentServer.directives[directive] = value;
			}
		}
		return config;
	}

private:
	std::string trim(const std::string &s)
	{
		size_t start = s.find_first_not_of(" \t");
		size_t end = s.find_last_not_of(" \t");
		return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
	}
};

int main()
{
	NginxParser parser;
	NginxConfig config = parser.parseConfig("empty.conf");

	for (size_t i = 0; i < config.servers.size(); ++i)
	{
		const Server &server = config.servers[i];
		std::cout << "Server " << i + 1 << ":\n";
		std::cout << "  server_name: " << server.server_name << "\n";
		std::cout << "  listen: " << server.listen << "\n";
		for (std::map<std::string, std::string>::const_iterator it = server.directives.begin(); it != server.directives.end(); ++it)
			std::cout << "  " << it->first << ": " << it->second << "\n";
		for (size_t j = 0; j < server.locations.size(); ++j)
		{
			const Location &location = server.locations[j];
			std::cout << "  Location " << j + 1 << ": " << location.path << "\n";
			for (std::map<std::string, std::string>::const_iterator it = location.directives.begin(); it != location.directives.end(); ++it)
				std::cout << "    " << it->first << ": " << it->second << "\n";
		}
	}
	return 0;
}
