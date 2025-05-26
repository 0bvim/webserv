#ifndef ASTREAM_HPP
#define ASTREAM_HPP

#include "IStream.hpp"
#include <ctime>
#include <string>

class AStream : public IStream
{
	protected:
		int _fd;
		std::string _id;
		std::string _ip;
		std::string _input;
		std::string _output;
		std::time_t _time;
		size_t _size;
		size_t _transfers;
		int _step;

		AStream(const int &fd, const std::string &id);
		AStream(const AStream &src);
		AStream &operator=(const AStream &rhs);

		virtual void processInput(const size_t &bytes);
		virtual void processOutput(const size_t &bytes);

	public:
		virtual ~AStream(void);

		int getFd(void) const;
		std::string getId(void) const;
		void setIp(const std::string &ip);
		std::string getIp(void) const;

		virtual void setData(std::vector<char> &buffer, const size_t &bytes);
		virtual std::string getData(const size_t &bytes);
		virtual size_t getSize(void) const;
		virtual void setStep(const int &step);
		virtual int getStep(void) const;
		virtual bool isTimedOut(void) const;
		virtual void updateTime(void);
};

#endif // ASTREAM_HPP
