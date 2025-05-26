#include "directive.hpp"
#include "Location.hpp"
#include "parser.hpp"
#include <iostream>
#include <ostream>
#include <string>

using namespace std;

Location::Location(void)
	: _deny_methods(false),
	  _autoindex(parser::AUTOINDEX_NOT_SET),
	  _webdav(parser::WEB_DAV_NOT_SET),
	  _max_body_size(0) {}

Location::Location(string &configuration_file) 
	: _deny_methods(false),
	  _autoindex(parser::AUTOINDEX_NOT_SET),
	  _webdav(parser::WEB_DAV_NOT_SET),
	  _max_body_size(0)
{
	parser::location(*this, configuration_file);
}

Location::Location(const Location &src)
{
	*this = src;
}

Location &Location::operator=(const Location &rhs)
{
	if (this == &rhs)
		return *this;

	_uri = rhs._uri;
	_allow_methods = rhs._allow_methods;
	_deny_methods = rhs._deny_methods;
	_root = rhs._root;
	_autoindex = rhs._autoindex;
	_webdav = rhs._webdav;
	_max_body_size = rhs._max_body_size;
	_indexes = rhs._indexes;
	_fastcgi = rhs._fastcgi;
	_extensions = rhs._extensions;
	_error_pages = rhs._error_pages;
	_return_code = rhs._return_code;
	_return_uri = rhs._return_uri;

	return *this;
}

Location::~Location(void) {}

bool Location::operator==(const Location &rhs) const
{
	return _uri == rhs._uri;
}

void Location::setURI(string uri)
{
	directive::setURI(uri, _uri);
}

string Location::getURI(void) const
{
	return _uri;
}

set<string> Location::getIndexes(void) const
{
	return _indexes;
}

void Location::addMethod(string method)
{
	directive::addMethod(method, _allow_methods);
}

void Location::setMethods(set<string> methods)
{
	_allow_methods = methods;
}

string Location::getMethod(string method) const
{
	set<string>::iterator tmp = _allow_methods.find(method);
	if (tmp == _allow_methods.end())
		return "";

	return *tmp;
}

set<string> Location::getMethods(void) const
{
	return _allow_methods;
}

void Location::setDenyMethods(string deny_methods)
{
	directive::setDenyMethods(deny_methods, _deny_methods);
}

bool Location::getDenyMethods(void) const
{
	return _deny_methods;
}

void Location::setRoot(string root)
{
	directive::setRoot(root, _root);
}

string Location::getRoot(void) const
{
	return _root;
}

void Location::setAutoIndex(string autoindex)
{
	directive::setAutoIndex(autoindex, _autoindex);
}

void Location::setAutoIndex(bitset<2> autoindex)
{
	_autoindex = autoindex;
}

bitset<2> Location::getAutoIndexBitSet(void) const
{
	return _autoindex;
}

bool Location::getAutoIndex(void) const
{
	return (_autoindex == parser::AUTOINDEX_ON ? true : false);
}

void Location::setWebDav(string webdav)
{
	directive::setWebDav(webdav, _webdav);
}

void Location::setWebDav(bitset<2> webdav)
{
	_webdav = webdav;
}

bitset<2> Location::getWebDavBitSet(void) const
{
	return _webdav;
}

bool Location::getWebDav() const
{
	return _webdav == parser::WEB_DAV_ON ? true : false;
}

void Location::setMaxBodySize(string max_body_size)
{
	directive::setMaxBodySize(max_body_size, _max_body_size);
}

size_t Location::getMaxBodySize(void) const
{
	return _max_body_size;
}

void Location::addIndex(string index)
{
	directive::addIndex(index, _indexes);
}

void Location::setIndexes(set<string> indexes)
{
	_indexes = indexes;
}

void Location::setFastCgi(std::string fastcgi)
{
	directive::setFastCgi(fastcgi, _fastcgi);
}

void Location::setFastCgiExtension(std::string extensions)
{
	directive::setFastCgiExtension(extensions, _extensions);
}

string Location::getFastCgi(void) const
{
	return _fastcgi;
}

set<string> Location::getFastCgiExtension(void) const
{
	return _extensions;
}

bool Location::isCgi(string extension) const
{
	if (_extensions.find(extension) == _extensions.end())
		return false;

	return true;
}

void Location::addErrorPages(string error_page)
{
	directive::addErrorPage(error_page, _error_pages);
}

void Location::setErrorPages(map<string, string> error_pages)
{
	_error_pages = error_pages;
}

map<string, string> Location::getErrorPages(void) const
{
	return _error_pages;
}

string Location::getErrorPageByCode(string code) const
{
	map<string, string>::const_iterator error_page  = _error_pages.find(code);
	if (error_page == _error_pages.end())
		return "";

	return error_page->second;
}

void Location::setReturn(string value)
{
	directive::setReturn(value, _return_code, _return_uri);
}

string Location::getReturnCode() const
{
	return _return_code;
}

string Location::getReturnURI() const
{
	return _return_uri;
}

bool Location::empty(void) const
{
	return _uri.empty();
}

ostream &operator<<(ostream &os, const Location &src)
{
	os << "\t\tlocation " << src.getURI() << " {" << endl;
	
	os << "\t\t\tindex";
	set<string> indexs = src.getIndexes();
	for (set<string>::iterator it = indexs.begin(); it != indexs.end(); it++)
		os << " " << *it;
	os << ";" << endl;

	os << "\t\t\troot " << src.getRoot() << ";" << endl;

	os << "\t\t\tlimit_except";
	set<string> methods = src.getMethods();
	for (set<string>::iterator it = methods.begin(); it != methods.end(); it++)
		os << " " << *it;
	os << " {";
	if (src.getDenyMethods())
		os << "\n\t\t\t\tdeny all;\n\t\t\t";
	os << "}" << endl;

	os << "\t\t\tclient_max_body_size " << src.getMaxBodySize() << ";" << endl;
	os << "\t\t\tautoindex " << (src.getAutoIndex() ? "on" : "off") << ";" << endl;
	os << "\t\t\twebdav " << (src.getWebDav() ? "on" : "off") << ";"<< endl;
	os << "\t\t\tfastcgi_pass " << src.getFastCgi() << ";" << endl;
	
	os << "\t\t\tfastcgi_extension ";
	set<string> extensions = src.getFastCgiExtension();
	for(set<string>::iterator it = extensions.begin(); it != extensions.end(); it++)
		os << *it << " ";
	os << ";" << endl;

	map<string, string> error_pages = src.getErrorPages();
	for (map<string, string>::iterator it = error_pages.begin(); it != error_pages.end(); it++)
		os << "\t\t\terror_page " << it->first << " " << (*it).second << ";" << endl;

	os << "\t\t\treturn " << src.getReturnCode() << " " << src.getReturnURI() << ";" << endl;
	os << "\t\t}";

	return os;
}
