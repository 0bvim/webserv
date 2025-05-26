#ifndef MIME_HPP
#define MIME_HPP

#include <map>
#include <string>

class Mime
{
	private:
		static Mime *_instance;

		std::string _default_mime;
		std::map<std::string, std::string> _mimes;

		void addMime(const std::string &key, const std::string &value);
		void parseMimes(std::string &buffer);

	public:
		Mime(void);
		~Mime(void);

		static Mime *getInstance(void);

		void configure(const std::string &filename);
		std::string getType(std::string extension) const;
};

#endif // MIME_HPP
