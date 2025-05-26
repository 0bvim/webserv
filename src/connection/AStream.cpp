#include "AStream.hpp"
#include "standard.hpp"
#include "step.hpp"
#include <ctime>
#include <string>

int _fd;
std::string _id;
std::string _buffer;
std::time_t _time;

using namespace std;

AStream::AStream(const int &fd, const std::string &id)
	: _fd(fd),
	  _id(id),
	  _time(time(NULL)),
	  _size(0),
	  _transfers(0),
	  _step(step::NONE) {}

AStream::AStream(const AStream &src)
{
	*this = src;
}

AStream &AStream::operator=(const AStream &rhs)
{
	if (this == &rhs)
		return *this;

	_fd = rhs._fd;
	_id = rhs._id;
	_ip = rhs._ip;
	_input = rhs._input;
	_output = rhs._output;
	_time = rhs._time;
	_size = rhs._size;
	_transfers = rhs._transfers;
	_step = rhs._step;

	return *this;
}

AStream::~AStream(void) {}

int AStream::getFd(void) const
{
	return _fd;
}

std::string AStream::getId(void) const
{
	return _id;
}

void AStream::setIp(const std::string &ip)
{
	_ip = ip;
}

std::string AStream::getIp(void) const
{
	return _ip;
}

void AStream::processInput(const size_t &bytes)
{
	(void) bytes;
}

void AStream::setData(std::vector<char> &buffer, const size_t &bytes)
{
	if (!bytes || buffer.empty())
		return processInput(bytes);

	_input.append(buffer.begin(), buffer.begin() + bytes);

	processInput(bytes);

	_time = time(NULL);
}

void AStream::processOutput(const size_t &bytes)
{
	(void) bytes;
}

std::string AStream::getData(const size_t &bytes)
{
	processOutput(bytes);

	if (_output.empty())
		return "";

	const string data = _output.substr(0, bytes);
	_output.erase(0, bytes);

	_time = time(NULL);

	return data;
}

size_t AStream::getSize(void) const
{
	return _size;
}

void AStream::setStep(const int &step)
{
	_step = step;
}

int AStream::getStep(void) const
{
	return _step;
}

bool AStream::isTimedOut(void) const
{
	size_t elapsed_time = time(NULL) - _time;
	if (elapsed_time >= standard::TIMEOUT)
		return true;

	return false;
}

void AStream::updateTime(void)
{
    _time = time(NULL);
}
