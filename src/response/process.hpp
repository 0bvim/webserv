#ifndef PROCESS_HPP
#define PROCESS_HPP

class Connection;
class Location;
class URL;

namespace process
{
	void request(Connection *connection);
	void methodGet(Connection *connection);
	void methodPost(Connection *connection);
	void methodDelete(Connection *connection);
	void deleteDirectory(Connection *connection, URL *uri);
	void deleteFile(Connection *connection, URL *uri);
	void methodDeleteCgi(Connection *connection);
};

#endif // PROCESS_HPP
