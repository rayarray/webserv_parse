#include <iostream>
#include "ConfigParser.hpp"

// main for testing configparser, reads default.conf in current directory

int main(void) {
	ConfigParser conf("default.conf");
	conf.getConfigSets();
	std::cout << std::boolalpha << "ConfigParser.startParse() = " << conf.startParse() << std::endl;
	std::cout << "MAIN: _line: [" << conf._error << "]" << std::endl;

	// example request resolve from server
	Server localhost("localhost", 80);
	Request request;
	const std::string request_hostname("localhost");
	const std::string request_path("/index.html");
	if (localhost.matchRequest(request_hostname, 80)) {
		request = localhost.resolveRequest(request_path);
		std::cout << "MAIN: request path: " << request.getPath() << std::endl;
	} 

}