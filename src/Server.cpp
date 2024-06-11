#include "ws_functions.hpp"
#include "Server.hpp"

Server::Server(const std::string listen, const size_t port) : ConfigSection("server"), _listen_name(listen), _port(port), _client_max_body_size(0) {}

void Server::initialize() {
	size_t idx;
	//std::cout << "SERVER INITIALIZE" << std::endl;
	printAll();
	if (doesLineExist("server_name", idx)) {
		for (size_t i = 1; !getIndexArg(idx, i).empty(); i++)
			_server_names.push_back(getIndexArg(idx, i));
	}
	if (doesLineExist("error_page", idx)) {
		std::cout << "ERROR_PAGE FOUND AT " << idx << std::endl;
		size_t pidx = idx;
		if (doesLineExist("error_page", idx, pidx))
			std::cout << "ERROR_PAGE FOUND AT " << idx << std::endl;
	}
	if (doesLineExist("error_page", idx)) {
		addErrorPage(getIndexArg(idx, 1), getIndexArg(idx, 2));
		size_t previous_idx = idx;
		while (doesLineExist("error_page", idx, previous_idx)) {
			addErrorPage(getIndexArg(idx, 1), getIndexArg(idx, 2));
			previous_idx = idx;
		}
	}
	if (doesLineExist("client_max_body_size", idx)) {
		_client_max_body_size = std::stoi(getIndexArg(idx, 1));
	}
}

bool Server::addErrorPage(const std::string nbr, const std::string file_path) {
	_error_pages.insert({ std::stoi(nbr), file_path });
	return true;
}

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
	return (location.initialize(), _locations.push_back(location), true);
}

void Server::addLocation(Location location) {
	_locations.push_back(location);
}

bool Server::matchRequest(const std::string server_name, const size_t port) {
	return (_listen_name.find(server_name) == 0 && port == _port);
}

Request Server::resolveRequest(const std::string request) {
	return Request("/usr/share/www" + request);
}

void Server::printData() {
	std::cout << "Server.printData() : " << _listen_name << ":" << _port << std::endl;
	if (_server_names.size() > 0) std::cout << "Names: ";
	for (const std::string &s : _server_names)
		std::cout << s << " | ";
	std::cout << std::endl;
	if (_error_pages.size() > 0) std::cout << "Error pages:" << std::endl;
	for (const std::pair<int, std::string> &p : _error_pages)
		std::cout << p.first << ":" << p.second << std::endl;
	for (Location &loc : _locations)
		loc.printAll();
}