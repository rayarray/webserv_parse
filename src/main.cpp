#include <iostream>
#include "ws_functions.hpp"
#include "ConfigParser.hpp"

// main for testing configparser, reads default.conf in current directory

int main(void) {
	//ConfigFile cf("server.conf");
	//cf.print();
	//while (cf.nextLine() && !cf.emptyLine() && cf.processLine()) cf.print();
	ConfigParser conf("server.conf");
	bool start_parse = conf.startParse();
	std::cout << std::boolalpha << "ConfigParser.startParse() = " << start_parse << std::endl;
	//Server serv = conf.getServer();
	std::vector<Server> servers;
	while (!conf.endParse()) {
		servers.push_back(conf.getServer());
		servers.back().printData();
	}
	{	Response response;
		Request request(1, "www.example.com", 80, "/cgi-bin");
		for (Server &srv : servers) {
			if (srv.matchRequest(request)) {
				response = srv.resolveRequest(request);
		}	}
		if (response.getType() != -100)
			std::cout << "\e[0;33mReq path [" << request._path << "] Response type [" << response.getType() << "] path [" << response.getPath() << "]\e[0m" << std::endl;
	}
	{	Response response;
		Request request(1, "www.example.com", 80, "/cgi-bin/view.py");
		for (Server &srv : servers) {
			if (srv.matchRequest(request)) {
				response = srv.resolveRequest(request);
		}	}
		if (response.getType() != -100)
			std::cout << "\e[0;33mReq path [" << request._path << "] Response type [" << response.getType() << "] path [" << response.getPath() << "]\e[0m" << std::endl;
	}
	{	Response response;
		Request request(REQ_POST, "www.example.com", 80, "/cgi-bin/special/specialcgi.py");
		for (Server &srv : servers) {
			if (srv.matchRequest(request)) {
				response = srv.resolveRequest(request);
		}	}
		if (response.getType() != -100)
			std::cout << "\e[0;33mReq path [" << request._path << "] Response type [" << response.getType() << "] path [" << response.getPath() << "]\e[0m" << std::endl;
	}
	{	Response response;
		Request request(2, "otherserver.com", 8080, "/foobar.html");
		for (Server &srv : servers) {
			if (srv.matchRequest(request)) {
				response = srv.resolveRequest(request);
		}	}
		if (response.getType() != -100)
			std::cout << "\e[0;33mReq path [" << request._path << "] Response type [" << response.getType() << "] path [" << response.getPath() << "]\e[0m" << std::endl;
		else 
			std::cout << "Failed to match response to server" << std::endl;
	}
	for (Server &srv : servers) {
		size_t page = 404;
		std::cout << "querying server [" << srv._listen_name << "] for error page " << page << ": " << srv.getErrorPage(page) << std::endl;
	}
	//bool end_parse = conf.endParse();
	//std::cout << "endParse retuns: " << std::boolalpha << end_parse << std::endl;
	//conf.printCS();
	//serv.printAll();
	//serv.addLocation();
	// / // * configreference and configfile testing
	// ConfigReference ref("config_defaults");
	// ref.print();
	// std::string s = "global", k = "test"; size_t i = 0;
	// std::cout << "keyParamType " << s << ":" << k << ":" << i << " = " << ref.keyParamType(s, k, i) << std::endl; i++;
	// std::cout << "keyParamType " << s << ":" << k << ":" << i << " = " << ref.keyParamType(s, k, i) << std::endl; i++;
	// std::cout << "keyParamType " << s << ":" << k << ":" << i << " = " << ref.keyParamType(s, k, i) << std::endl; i++;
	// std::cout << "keyParamType " << s << ":" << k << ":" << i << " = " << ref.keyParamType(s, k, i) << std::endl; i++;
	// std::cout << "keyParamType " << s << ":" << k << ":" << i << " = " << ref.keyParamType(s, k, i) << std::endl;

	// ConfigFile cfg("server.conf");
	// while (cfg.nextLine()) {
	// 	cfg.processLine();
	// 	std::cout << "{" << cfg.getSection() << "}";
	// 	for (size_t i = 0; !cfg.getWord(i).empty(); i++)
	// 		std::cout << "[" << cfg.getWord(i) << "]";
	// 	std::cout << std::endl;
	// }
	

	// * old main
	// ConfigParser conf("server.conf");
	// bool start_parse = conf.startParse();
	// std::cout << std::boolalpha << "ConfigParser.startParse() = " << start_parse << std::endl;
	// if (!start_parse)
	// 	std::cout << "MAIN: conf._error: " << conf._error << std::endl;
	// //std::cout << "MAIN: _line: [" << conf._error << "]" << std::endl;
	// conf.printCS();

	// // example request resolve from server
	// Server localhost("localhost", 80);
	// Request request;
	// const std::string request_hostname("localhost");
	// const std::string request_path("/index.html");
	// if (localhost.matchRequest(request_hostname, 80)) {
	// 	request = localhost.resolveRequest(request_path);
	// 	//std::cout << "MAIN: request path: " << request.getPath() << std::endl;
	// }

	// /* // * testing ws_getarg() */
	// std::string s("test N T C");
	// std::cout << "ws_getarglen \"" << s << "\": " << ws_getarglen(s) << std::endl;
	// std::cout << "ws_getarg 1 \"" << s << "\": " << ws_getarg(1, s) << std::endl;
	// std::cout << "ws_getarg 2 \"" << s << "\": " << ws_getarg(2, s) << std::endl;
	// std::cout << "ws_getarg 3 \"" << s << "\": " << ws_getarg(3, s) << std::endl;
	// std::cout << "ws_getarg 4 \"" << s << "\": " << ws_getarg(4, s) << std::endl;
	// std::cout << "ws_getarg 5 \"" << s << "\": " << ws_getarg(4, s) << std::endl;

	// /* // * testing ConfigSection and inheritance */
	// localhost.addConfigLine(std::vector<std::string>{"listen", "one.example.com"});
	// localhost.addConfigLine({"location", "/"});
	// std::cout << localhost.getIndexArg(0, 0) << " " << localhost.getIndexArg(0, 1) << std::endl;
	// std::cout << localhost.getIndexArg(1, 0) << " " << localhost.getIndexArg(1, 1) << std::endl;

	// /* // * testing ws_getargstr */
	// std::string ss("test foo bar 543543 z\"bc\"fsfs\"\" #fabadoo");
	// std::cout << "ws_getargstr: " << ss << " 1: " << ws_getargstr(1, ss) << std::endl;
	// std::cout << "ws_getargstr: " << ss << " 2: " << ws_getargstr(2, ss) << std::endl;
	// std::cout << "ws_getargstr: " << ss << " 3: " << ws_getargstr(3, ss) << std::endl;
	// std::cout << "ws_getargstr: " << ss << " 4: " << ws_getargstr(4, ss) << std::endl;

	// std::vector<std::string> list = {"foo", "bar", "FOOBAR", "baz"};
	// size_t i = 0;
	// ws_checkword_lower("baz", list, i, 2);
	// std::cout << "ws_checkword_lower(\"baz\", foo|bar|FOOBAR|baz, i, 2) = " << i << std::endl;
}