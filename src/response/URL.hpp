#ifndef URL_HPP
#define URL_HPP

#include <string>
#include <ostream>

class Connection;
class Location;

class URL
{
	private:
		enum {
			READ = 1,
			WRITE = 2,
			EXECUTE = 4,
			FILE = 8,
			DIRECTORY = 16,
			CGI = 32,
			DELETE = 64
		};

		Connection *_connection;
		std::string _scheme;
		std::string _host;
		std::string _port;
		std::string _path;
		std::string _file;
		std::string _extension;
		std::string _path_info;
		std::string _query;
		int _dac;

		std::string checkIndex(const Location &location, std::string &path);

		void processPath(std::string path);

		bool _isDirectory(const std::string &path);
		bool _isFile(const std::string &path);
		bool _isCgi(const std::string &path);

		bool _isReadable(const std::string &path);
		bool _isWritable(const std::string &path);
		bool _isExecutable(const std::string &path);
		bool _isDirectoryEmpty(const std::string &path);
		bool _isDeletable(const std::string &path);

		void checkDAC(const std::string &path);

	public:
		URL(Connection *connection);
		URL(const URL &src);
		URL &operator=(const URL &rhs);
		~URL(void);

		std::string getScheme(void) const;
		std::string getHost(void) const;
		std::string getPort(void) const;
		std::string getPath(void) const;
		std::string getFile(void) const;
		std::string getExtension(void) const;
		std::string getPathInfo(void) const;
		std::string getPathTranslated(void) const;
		std::string getQuery(void) const;
		std::string getAbsolutePath(void) const;
		std::string getLocation(void) const;
		
		bool isDirectory(void) const;
		bool isFile(void) const;
		bool isCgi(void) const;

		bool isReadable(void) const;
		bool isWritable(void) const;
		bool isExecutable(void) const;
		bool isDeletable(void) const;

		static void decode(std::string &path);
};

std::ostream &operator<<(std::ostream &os, const URL &src);

#endif // URL_HPP
