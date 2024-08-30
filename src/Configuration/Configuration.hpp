#pragma once

#include <vector>
#include <map>
#include <string>

class Configuration
{
public:
	Configuration();
	bool loadFromFile(const std::string &filename);

	std::vector<int> getPorts() const;
	std::string getHost() const;
	std::map<std::string, std::string> getRoutes() const;

private:
	std::vector<int> ports;
	std::string host;
	std::map<std::string, std::string> routes;

	void parseLine(const std::string &line);
	void addRoute(const std::string &path, const std::string &file);
};
