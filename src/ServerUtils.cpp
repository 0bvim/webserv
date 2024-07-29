#include "../include/Server.hpp"

void Server::_fillBuffer(int fd, const char *str)
{
	std::string buff(str);
	if (this->_buffer_request.find(fd) != this->_buffer_request.end())
		this->_buffer_request[fd] += buff;
	else
	{
		if (buff.find("HTTP/1.1") == std::string::npos)
		{
			this->_status = HttpStatus::BAD_REQUEST;
			return;
		}
		this->_buffer_request[fd] = buff;
	}
}

bool Server::_checkEndMessage(int fd)
{
	std::string &str = this->_buffer_request[fd];
	return (str.find("\r\n\r\n") != (std::string::npos));
}

void Server::_printOnClient(int fd, std::string const &str)
{
	ssize_t bytes_written = write(fd, str.c_str(), str.length());
	if (bytes_written == -1)
		std::cerr << "Write error" << std::endl;
	this->_buffer_request.erase(fd);
	this->_status = HttpStatus::ZERO;
}

ssize_t Server::_readFromClient(int fd, char *buff)
{
	ssize_t bytes_read = read(fd, buff, BUFFER_SIZE - 1);
	if (bytes_read == -1)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return 0;
		std::cerr << "Read error: " << strerror(errno) << std::endl;
		close(fd);
	}
	else if (bytes_read == 0)
		close(fd);
	return (bytes_read);
}
