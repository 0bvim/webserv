#include "Mime.hpp"
#include "logger.hpp"
#include "parser.hpp"
#include "standard.hpp"
#include <fstream>
#include <list>
#include <map>
#include <string>

using namespace std;

Mime *Mime::_instance = NULL;

Mime::Mime(void) : _default_mime(standard::MIME) {}

Mime::~Mime(void) {}

Mime *Mime::getInstance(void)
{
	if (_instance == NULL)
		_instance = new Mime();

	return _instance;
}

void Mime::configure(const std::string &filename)
{
	try
	{
		_mimes.clear();

		ifstream file(filename.c_str());
		if (not file.is_open())
			throw runtime_error("failed to open json file: " + filename);

		string buffer;

		for (string line; getline(file, line); buffer.append(line))
		{
			if (line.find("#") != string::npos)
				line = line.substr(0, line.find_first_of("#"));

			parser::trim(line, " \n\t\r\v\f");
		}

		file.close();

		parser::replace(buffer, '\t', ' ');

		parser::erase(buffer, "  ", 1);
		parser::erase(buffer, " [", 1);
		parser::rerase(buffer, "] ", 1);
		parser::erase(buffer, " ]", 1);
		parser::rerase(buffer, "[ ", 1);
		parser::erase(buffer, " {", 1);
		parser::rerase(buffer, "{ ", 1);
		parser::erase(buffer, " }", 1);
		parser::rerase(buffer, "} ", 1);
		parser::erase(buffer, " ,", 1);
		parser::rerase(buffer, ", ", 1);

		parseMimes(buffer);
	}
	catch (exception &e)
	{
		throw runtime_error(string("failed to parse json at: ") + e.what());
	}

	logger::info("mimes json file parsed: " + filename);
}

void Mime::addMime(const string &key, const string &values) {

	list<string> extensions = parser::split(values, ',');

	list<string>::iterator extension = extensions.begin();
	for (; extension != extensions.end(); ++extension)
	{
		string value = parser::find("\"", *extension, "\"");
		if (value.empty() || extension->size())
			throw runtime_error("empty value at key: \"" + key + "\"");

		_mimes[value] = key;
	}
}

void Mime::parseMimes(string &buffer)
{
	if (parser::compare("{", buffer))
		buffer.erase(0, 1);
	else
		throw runtime_error("unclosed json");

	size_t npos = buffer.find_last_of("}");
	if (npos != string::npos)
		buffer.erase(npos, 1);
	else
	 	throw runtime_error("unclosed json");

	if (buffer.empty())
		return;
	
	string key, value;

	for (size_t i = buffer.size(); i > 0; --i)
	{

		key = parser::find("\"", buffer, "\"");
		if (key.empty())
			throw runtime_error("empty key: \"" + key + "\"" + buffer);
		if (buffer.at(0) == ':')
			buffer.erase(0, 1);
		else
			throw runtime_error("missing \":\"");

		value = parser::find("[", buffer, "]");
		if (value.empty())
			throw runtime_error("empty value at key: \"" + key + "\"");
		if (buffer.empty() || buffer.at(0) == ',')
		{
			addMime(key, value);

			if (buffer.empty())
				return;

			buffer.erase(0, 1);
			continue;
		}
		throw runtime_error("missing \",\" at: " + buffer);
	}
}

string Mime::getType(string extension) const
{
	size_t pos = extension.find_last_of(".");
	if (pos == string::npos)
		return _default_mime;

	extension.erase(0, pos + 1);

	map<string, string>::const_iterator mime = _mimes.find(extension);
	if (mime == _mimes.end())
		return _default_mime;

	return mime->second;
}
