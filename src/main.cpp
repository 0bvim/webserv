#include "Http.hpp"
#include "logger.hpp"
#include "Mime.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>

using namespace std;

static void handle_signal(int signal)
{
	Http::getInstance()->stop(signal);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, handle_signal);

	int status = EXIT_SUCCESS;

	Http *http = Http::getInstance();

	try
	{
		if (argc > 2)
			throw std::runtime_error("too many configuration files");

		string file = (argv[1] ? argv[1] : "./configurations/default.conf");

		http->configure(file);

		Mime::getInstance()->configure("./src/parser/mime/mimes.json");

		http->start();

	}
	catch (std::exception &exception)
	{
		logger::fatal(exception.what());
		status = http->getSignal();
	}

	delete http;

	return status;
}
