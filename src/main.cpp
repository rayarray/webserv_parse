#include <iostream>
#include "ws_functions.hpp"
#include "ConfigParser.hpp"

// main for testing configparser, reads default.conf in current directory

int main(void) {
	ConfigParser conf("server.conf");
	bool start_parse = conf.startParse();
	std::cout << std::boolalpha << "ConfigParser.startParse() = " << start_parse << std::endl;
	if (!start_parse)
		std::cout << "MAIN: conf._error: " << conf._error << std::endl;
	//std::cout << "MAIN: _line: [" << conf._error << "]" << std::endl;

	// example request resolve from server
	Server localhost("localhost", 80);
	Request request;
	const std::string request_hostname("localhost");
	const std::string request_path("/index.html");
	if (localhost.matchRequest(request_hostname, 80)) {
		request = localhost.resolveRequest(request_path);
		//std::cout << "MAIN: request path: " << request.getPath() << std::endl;
	}
	//std::cout << "ws_toupper test: " << ws_toupper("server") << std::endl;

	// // * testing ws_getarg()
	// std::string s("test N T C");
	// std::cout << "ws_getarglen \"" << s << "\": " << ws_getarglen(s) << std::endl;
	// std::cout << "ws_getarg 1 \"" << s << "\": " << ws_getarg(1, s) << std::endl;
	// std::cout << "ws_getarg 2 \"" << s << "\": " << ws_getarg(2, s) << std::endl;
	// std::cout << "ws_getarg 3 \"" << s << "\": " << ws_getarg(3, s) << std::endl;
	// std::cout << "ws_getarg 4 \"" << s << "\": " << ws_getarg(4, s) << std::endl;
	// std::cout << "ws_getarg 5 \"" << s << "\": " << ws_getarg(4, s) << std::endl;

	// * testing ConfigSection and inheritance
	localhost.addConfigLine(std::vector<std::string>{"listen", "one.example.com"});
	localhost.addConfigLine({"location", "/"});
	std::cout << localhost.getIndexArg(0, 0) << " " << localhost.getIndexArg(0, 1) << std::endl;
	std::cout << localhost.getIndexArg(1, 0) << " " << localhost.getIndexArg(1, 1) << std::endl;
}