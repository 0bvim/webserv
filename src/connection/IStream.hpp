#ifndef ISTREAM_HPP
#define ISTREAM_HPP

#include <string>
#include <vector>

class IStream
{
	public:
		virtual ~IStream(void) {};

		virtual int getFd(void) const = 0;
		virtual std::string getId(void) const = 0;
		virtual void setIp(const std::string &ip) = 0;
		virtual std::string getIp(void) const = 0;

		virtual void setData(std::vector<char> &buffer, const size_t &bytes) = 0;
		virtual std::string getData(const size_t &bytes) = 0;
		virtual size_t getSize(void) const = 0;
		virtual void setStep(const int &step) = 0;
		virtual int getStep(void) const = 0;
		virtual bool isTimedOut(void) const = 0;
};

#endif // ISTREAM_HPP
