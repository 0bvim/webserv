#include "Directory.hpp"
#include "Connection.hpp"
#include "Resource.hpp"
#include "step.hpp"
#include <algorithm>
#include <dirent.h>

using namespace std;

Directory::Directory(Connection *connection) : Resource(connection)
{
	_type = "text/html";
	
	DIR *dir;
	struct dirent *ent;
	const string &directory = connection->getUri()->getPath();

	_output = std::string(
			"<html>\n"
			"<head>"
			"<title>Index of " + directory + "</title>"
			"<script src=\"https://cdn.tailwindcss.com\"></script>"
			"</head>\n"
			"<body class=\"bg-gray-100 text-gray-900 min-h-screen\">\n"
			"<div class=\"container mx-auto p-4\">\n"
			"<div class=\"bg-white text-gray-900 rounded-lg shadow-lg p-8 max-w-3xl w-full mx-auto mt-8\">\n"
			"<h1 class=\"text-3xl font-bold mb-4\">Index of " + directory + "</h1>\n"
			"<div class=\"space-y-2\">\n");

	std::vector<std::string> entries;

	if ((dir = opendir(connection->getUri()->getAbsolutePath().c_str())) != NULL)
	{
		while ((ent = readdir(dir)) != NULL)
		{
			std::string name = ent->d_name;

			if (name == ".")
				continue;

			entries.push_back(name);
		}

		closedir(dir);
	}

	std::sort(entries.begin(), entries.end());

	vector<string>::iterator entry = entries.begin();
	for (; entry != entries.end(); ++entry)
	{
		_output += "<a "
			"href=\"" + directory + *entry + "\" "
			"class=\"text-blue-500 hover:underline text-lg block\">" + *entry + "</a>\n";
	}

	_output += "</div>\n</div>\n</div>\n</body>\n</html>\n";

	_size = _output.size();
	_step = step::CLOSE;
}

Directory::Directory(const Directory &src) : Resource(src._connection)
{
	*this = src;
}

Directory &Directory::operator=(const Directory &rhs)
{
	if (this == &rhs)
		return *this;

	return *this;
}

Directory::~Directory(void) {}
