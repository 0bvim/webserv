#include "Configuration.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Configuration::Configuration() : host("127.0.0.1")
{
	// Default values (host can be modified)
}

bool Configuration::loadFromFile(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Could not open configuration file: " << filename << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line))
	{
		parseLine(line);
	}

	file.close();
	return true;
}

void Configuration::parseLine(const std::string &line)
{
	std::istringstream iss(line);
	std::string key, value;

	if (!(iss >> key >> value))
	{
		return; // Skip empty or malformed lines
	}

	if (key == "port")
	{
		ports.push_back(std::stoi(value));
	}
	else if (key == "host")
	{
		host = value;
	}
	else if (key == "route")
	{
		std::string path, file;
		iss >> path >> file;
		addRoute(path, file);
	}
}

void Configuration::addRoute(const std::string &path, const std::string &file)
{
	routes[path] = file;
}

std::vector<int> Configuration::getPorts() const
{
	return ports;
}

std::string Configuration::getHost() const
{
	return host;
}

std::map<std::string, std::string> Configuration::getRoutes() const
{
	return routes;
}
