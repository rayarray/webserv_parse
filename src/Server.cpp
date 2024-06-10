#include "ws_functions.hpp"
#include "Server.hpp"

Server::Server(const std::string listen, const size_t port) : ConfigSection("server"), _listen_name(listen), _port(port) {}

bool Server::addLocation() {
	size_t idx;
	std::cout << "srv:addLoc:doesLineExist('location') :" << std::boolalpha << doesLineExist("location", idx) << std::endl;
	Location location(getIndexArg(idx, 1));
	while (getIndexArg(idx, 1) != "}") {
		for (size_t i = 0;!getIndexArg(idx, i).empty();i++)
			std::cout << getIndexArg(idx, i) << ":";
		std::cout << std::endl;
		idx++;
	}
	std::cout << "addLoc end: " << getIndexArg(idx, 0) << "-" << getIndexArg(idx, 1) << std::endl;
	return true;
}

bool Server::matchRequest(const std::string server_name, const size_t port) {
	return (_listen_name.find(server_name) == 0 && port == _port);
}

Request Server::resolveRequest(const std::string request) {
	return Request("/usr/share/www" + request);
}