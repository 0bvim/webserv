#include "../include/Config.hpp"
#include <stdexcept>

Config::Config(const std::string &filePath) : filePath(filePath)
{
	this->checkFile();
	parseConfigFile();
}

std::vector<ServerConfig> const &Config::getServers() const
{
	return this->servers;
}

static void checkLinesContent(const std::vector<std::string> &vec)
{
	std::vector<std::string>::const_iterator it;

	for (it = vec.begin(); it != vec.end(); ++it)
		if (it->find("server {") != std::string::npos)
			return;
	throw std::runtime_error("Server Not Found");
}

void Config::parseConfigFile()
{
	std::ifstream configFile(filePath.c_str());
	if (!configFile.is_open())
	{
		std::cerr << "Error opening config file: " << filePath << std::endl;
		return;
	}
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(configFile, line))
		lines.push_back(line);
	if (!lines.size())
		throw std::runtime_error("File is empty \U0001F919");
	checkLinesContent(lines);
	try
	{
		size_t index = 0;
		checkBraces(lines);
		while (index < lines.size())
		{
			std::string trimmedLine = trim(lines[index]);
			if (trimmedLine == "server {")
				parseServerBlock(lines, index);
			++index;
		}
	}
	catch (std::exception &e)
	{
		std::cerr << "Error parsing config file: " << e.what() << std::endl;
	}
}

void Config::parseServerBlock(const std::vector<std::string> &lines, size_t &index)
{
	ServerConfig server;

	while (++index < lines.size())
	{
		std::string trimmedLine = trim(lines[index]);
		if (trimmedLine == "}")
			break;
		if (!trimmedLine.empty() &&
			trimmedLine[trimmedLine.size() - 1] != ';' &&
			trimmedLine.find("location") == std::string::npos)
		{
			throw std::runtime_error("Invalid line in server block");
		}
		else if (trimmedLine.find("server_name") == 0)
			server.server_name = trimmedLine.substr(12).erase(trimmedLine.size() - 13);
		else if (trimmedLine.find("listen") == 0)
		{
			std::string  str;
			std::istringstream iss(trimmedLine.substr(7).erase(trimmedLine.size() - 8));
			iss >> str;
			server.address = str.substr(0, str.find(":"));
			server.port = atoi(str.substr(str.find(":") + 1).c_str());
		}
		else if (trimmedLine.find("client_max_body_size") == 0)
		{
			std::istringstream iss(trimmedLine.substr(21).erase(trimmedLine.size() - 22));
			iss >> server.client_max_body_size;
		}
		else if (trimmedLine.find("error_page") == 0)
		{
			size_t pos = trimmedLine.find(' ', 11);
			int errorCode;
			std::istringstream iss(trimmedLine.substr(11, pos - 11));
			iss >> errorCode;
			std::string errorPage = trimmedLine.substr(pos + 1).erase(trimmedLine.size() - pos - 2);
			server.error_pages[errorCode] = errorPage;
		}
		else if (trimmedLine.find("location") == 0)
		{
			try
			{
				parseLocationBlock(lines, index, server);
			}
			catch (std::exception &e)
			{
				throw std::runtime_error(e.what());
			}
		}
	}
	servers.push_back(server);
}

void Config::parseLocationBlock(const std::vector<std::string> &lines, size_t &index, ServerConfig &server)
{
	LocationConfig location;

	std::string trimmedLine = trim(lines[index]);
	size_t pos = trimmedLine.find(' ');
	location.path = trim(trimmedLine.substr(pos + 1, trimmedLine.size() - pos - 2));

	while (++index < lines.size())
	{
		std::string trimmedLine = trim(lines[index]);
		if (trimmedLine == "}")
			break;
		if (!trimmedLine.empty() &&
			trimmedLine[trimmedLine.size() - 1] != ';')
		{
			throw std::runtime_error("Invalid line in server block");
		}
		else if (trimmedLine.find("autoindex") == 0)
			location.autoindex = (trimmedLine.substr(10) == "on");
		else if (trimmedLine.find("root") == 0)
			location.root = trimmedLine.substr(5).erase(trimmedLine.size() - 6);
		else if (trimmedLine.find("upload_dir") == 0)
			location.upload_dir = trimmedLine.substr(11).erase(trimmedLine.size() - 12);
		else if (trimmedLine.find("index") == 0)
		{
			std::istringstream iss(trimmedLine.substr(6).erase(trimmedLine.size() - 7));
			std::string idx;
			while (iss >> idx)
				location.index.push_back(idx);
		}
		else if (trimmedLine.find("allow_methods") == 0)
		{
			std::istringstream iss(trimmedLine.substr(14).erase(trimmedLine.size() - 15));
			std::string method;
			while (iss >> method)
				location.allow_methods.push_back(method);
		}
		else if (trimmedLine.find("cgi ") == 0)
		{
			std::vector<std::string> tokens = split(trimmedLine, ' ');
			if (tokens.size() != 3)
				throw std::runtime_error("Invalid cgi line");
			location.cgi.push_back(t_cgi_config());
			location.cgi.back().extension = tokens[1];
			location.cgi.back().path = tokens[2];
		}
		else if (trimmedLine.find("redirect") == 0)
			location.redirect = trimmedLine.substr(9).erase(trimmedLine.size() - 10);
	}
	server.locations.push_back(location);
}

bool Config::checkBraces(const std::vector<std::string> &lines)
{
	int openBraces = 0;

	for (size_t i = 0; i < lines.size(); ++i)
	{
		const std::string &line = lines[i];
		for (size_t j = 0; j < line.size(); ++j)
		{
			char c = line[j];
			if (c == '{')
				openBraces++;
			else if (c == '}')
			{
				if (openBraces == 0)
					throw std::runtime_error("Unmatched closing brace '}' found.");
				openBraces--;
			}
		}
	}
	if (openBraces != 0)
		throw std::runtime_error("Unmatched opening brace '{' found.");
	return true;
}

void Config::printServers() const
{
	std::cout << "Number of servers: " << servers.size() << "\n\n";
	for (size_t i = 0; i < servers.size(); ++i)
	{
		const ServerConfig &server = servers[i];
		std::cout << "Server Name: " << server.server_name << "\n";
		std::cout << "Listen: " << server.listen << "\n";
		std::cout << "Client Max Body Size: " << server.client_max_body_size << "\n";

		for (std::map<int, std::string>::const_iterator it = server.error_pages.begin(); it != server.error_pages.end(); ++it)
			std::cout << "Error Page " << it->first << ": " << it->second << "\n";

		for (size_t j = 0; j < server.locations.size(); ++j)
		{
			const LocationConfig &location = server.locations[j];
			std::cout << "Location: " << location.path << "\n";
			std::cout << "  Autoindex: " << (location.autoindex ? "on" : "off") << "\n";
			std::cout << "  Root: " << location.root << "\n";
			std::cout << "  Upload Dir: " << location.upload_dir << "\n";
			std::cout << "  Index: ";
			for (size_t k = 0; k < location.index.size(); ++k)
				std::cout << location.index[k] << " ";
			std::cout << "\n";
			std::cout << "  Allow Methods: ";
			for (size_t k = 0; k < location.allow_methods.size(); ++k)
				std::cout << location.allow_methods[k] << " ";
			std::cout << "\n";
			std::cout << "  Redirect: " << location.redirect << "\n";
		}
		std::cout << "\n";
	}
}
