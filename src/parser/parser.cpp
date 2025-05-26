#include "parser.hpp"
#include "Http.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <sstream>
#include <string>

using namespace std;

const char &parser::lastCharacter(const string &text) {
	return text.at(text.size() - 1);
}

size_t parser::toSizeT(const string &value) {
	stringstream ss;
	size_t tmp = 0;

	ss << value;
	ss >> tmp;

	return tmp;
}

string parser::toString(const size_t &value) {
	stringstream ss;

	ss << value;

	return ss.str();
}

string parser::toUpper(string text) {
	for (size_t i = 0; i < text.size(); i++)
		text.at(i) = toupper(text.at(i));

	return text;
}

string parser::toLower(string text) {
	for (size_t i = 0; i < text.size(); i++)
		text.at(i) = tolower(text.at(i));

	return text;
}

void parser::replace(string &buffer, const char &from, const char &to) {
	for (size_t i = 0; i < buffer.length(); i++)
		if (buffer.at(i) == from)
			buffer.at(i) = to;
}

void parser::erase(string &buffer, const string &text, const size_t &quantity) {
	size_t pos = buffer.find(text);

	while (pos != string::npos) {
		buffer.erase(pos, quantity);

		pos = buffer.find(text);
	}
}

void parser::rerase(string &buffer, const string &text,
										const size_t &quantity) {
	size_t pos = buffer.find(text);

	while (pos != string::npos) {
		buffer.erase(pos + text.size() - quantity, quantity);

		pos = buffer.find(text);
	}
}

void parser::trim(string &buffer, const string &set) {
	if (buffer.find_first_not_of(set) == string::npos)
		return buffer.clear();

	buffer = buffer.substr(buffer.find_first_not_of(set), buffer.size());
	buffer = buffer.substr(0, buffer.find_last_not_of(set) + 1);
}

bool parser::compare(const string &key, string &buffer) {
	if (buffer.empty())
		return false;

	if (buffer.compare(0, key.size(), key) == 0)
		return true;

	return false;
}

string parser::find(const string &key, string &buffer, const string &delimiter)
{
	if (buffer.empty())
		return "";

	if (not parser::compare(key, buffer))
		return "";

	buffer.erase(0, key.size());

	size_t pos = buffer.find(delimiter);

	if (pos == string::npos)
		return "";

	string tmp = buffer.substr(0, pos);
	buffer.erase(0, pos + 1);

	return tmp;
}

list<string> parser::split(string text, const char &delimiter)
{
	list<string> tmp;

	while (text.size())
	{
		parser::trim(text, string(1, delimiter));
		size_t pos = text.find_first_of(string(1, delimiter));

		if (not text.empty() && pos == string::npos)
		{
			tmp.push_back(text.substr(0, text.size()));
			text.erase(0, text.size());
		}
		else if (not text.empty() && pos != string::npos)
		{
			tmp.push_back(text.substr(0, pos));
			text.erase(0, pos);
		}
	}

	return tmp;
}

string parser::basename(const string &text)
{
	if (text.empty())
		return "";

	size_t pos = text.find_last_of(".");

	if (pos == string::npos)
		return "";

	return text.substr(pos, text.size() - pos);
}

string parser::formatPath(const string &path)
{
	list<string> new_files;
	list<string> files = parser::split(path, '/');

	list<string>::iterator file = files.begin();
	for (; file != files.end(); ++file) {

		if (*file == ".")
			continue;

		if (*file == "..") {
			if (new_files.size())
				new_files.pop_back();

			continue;
		}

		new_files.push_back(*file);
	}

	string new_path;

	for (file = new_files.begin(); file != new_files.end(); ++file)
		new_path += "/" + *file;

	return new_path + (parser::lastCharacter(path) == '/' ? "/" : "");
}

void parser::http(Http &http, string &buffer)
{
	if (parser::compare("http{", buffer))
	{
		buffer.erase(0, 5);

		size_t npos = buffer.find_last_of("}");
		if (npos != string::npos)
			buffer.erase(npos, 1);
	}

	for (size_t i = buffer.size(); i > 0; --i)
	{
		http.setMaxBodySize(find("client_max_body_size ", buffer, ";"));
		http.setRoot(find("root ", buffer, ";"));
		http.setAutoIndex(find("autoindex ", buffer, ";"));
		http.setWebDav(find("webdav ", buffer, ";"));
		http.addIndex(find("index ", buffer, ";"));
		http.addErrorPage(find("error_page ", buffer, ";"));

		if (parser::compare("server{", buffer))
			http.addServer(Server(buffer));

		if (buffer.empty())
		{
			buffer.erase(0, 1);
			return;
		}
	}

	throw runtime_error("failed to parse http at: " + buffer);
}

void parser::server(Server &server, string &buffer)
{
	buffer.erase(0, 7);

	for (size_t i = buffer.size(); i > 0; --i)
	{
		server.addListen(parser::find("listen ", buffer, ";"));
		server.addName(parser::find("server_name ", buffer, ";"));
		server.setRoot(parser::find("root ", buffer, ";"));
		server.addErrorPage(parser::find("error_page ", buffer, ";"));
		server.addIndex(parser::find("index ", buffer, ";"));
		server.setMaxBodySize(parser::find("client_max_body_size ", buffer, ";"));
		server.setAutoIndex(find("autoindex ", buffer, ";"));
		server.setWebDav(find("web_dav ", buffer, ";"));
		server.setReturn(parser::find("return ", buffer, ";"));

		if (parser::compare("location ", buffer))
			server.addLocation(Location(buffer));

		if (parser::compare("}", buffer))
		{
			buffer.erase(0, 1);
			return;
		}
	}

	throw runtime_error("failed to parser server at: " + buffer);
}

void parser::location(Location &location, string &buffer)
{
	location.setURI(find("location ", buffer, "{"));

	for (size_t i = buffer.size(); i > 0; --i)
	{
		location.addIndex(find("index ", buffer, ";"));
		location.setRoot(find("root ", buffer, ";"));
		location.setMaxBodySize(find("client_max_body_size ", buffer, ";"));
		location.setAutoIndex(find("autoindex ", buffer, ";"));
		location.setWebDav(find("web_dav ", buffer, ";"));
		location.setFastCgi(find("fastcgi_pass ", buffer, ";"));
		location.addErrorPages(find("error_page ", buffer, ";"));
		location.setReturn(find("return ", buffer, ";"));
		location.setFastCgiExtension(find("fastcgi_extension ", buffer, ";"));

		if (parser::compare("limit_except", buffer))
			parser::limit_except(location, buffer);

		if (parser::compare("}", buffer))
		{
			buffer.erase(0, 1);
			return;
		}
	}

	throw runtime_error("failed to parse location at: " + buffer);
}
void parser::limit_except(Location &location, string &buffer)
{
	for (size_t i = buffer.size(); i > 0; --i)
	{
		if (parser::compare("limit_except{", buffer))
			buffer.erase(0, 13);
		else if (parser::compare("limit_except ", buffer))
			location.addMethod(find("limit_except ", buffer, "{"));

		location.setDenyMethods(find("deny ", buffer, ";"));

		if (parser::compare("}", buffer))
		{
			buffer.erase(0, 1);
			return;
		}
	}

	throw runtime_error("failed to parse limit_except at: " + buffer);
}
