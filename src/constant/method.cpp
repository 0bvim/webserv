#include "method.hpp"

std::set<std::string> &method::getAllowedMethods(void)
{
	static std::set<std::string> default_methods;
	if (default_methods.empty())
	{
		default_methods.insert(method::GET);
		default_methods.insert(method::POST);
		default_methods.insert(method::DELETE);
	}

	return default_methods;
}
