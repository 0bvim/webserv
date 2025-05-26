#include "Connection.hpp"
#include "File.hpp"
#include "step.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

File::File(Connection *connection) : Resource(connection)
{
	_id = connection->getUri()->getAbsolutePath();

	_file.open(_id.c_str(), ios::binary);
	if (!_file.is_open())
		return;

	_file.seekg(0, ios::end);
	_size = _file.tellg();
	_file.seekg(0, ios::beg);

	_step = step::CLOSE;
}

File::File(const File &src) : Resource(src._connection)
{
	*this = src;
}

File &File::operator=(const File &rhs)
{
	if (this == &rhs)
		return *this;

	return *this;
}

File::~File(void)
{
	_file.close();
}

void File::processOutput(const size_t &bytes)
{
	if (!_file.is_open())
		return;

	vector<char> buffer(bytes);
	_file.read(buffer.data(), bytes);

	string data = "";
	vector<char>::iterator character = buffer.begin();
	for (; character != buffer.begin() + _file.gcount(); ++character)
		data += *character;

	_output.append(data);
}
