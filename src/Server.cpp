#include "ws_functions.hpp"
#include "Server.hpp"

Server::Server(const std::string listen, const size_t port) : ConfigSection("server"), _listen_name(listen), _port(port), _max_client_body_size(0) {}

void Server::initialize() {
	size_t idx;
	//std::cout << "SERVER INITIALIZE" << std::endl;
	printAll();
	if (doesLineExist("server_name", idx)) {
		for (size_t i = 1; !getIndexArg(idx, i).empty(); i++)
			_server_names.push_back(getIndexArg(idx, i));
	}
	if (doesLineExist("error_page", idx)) {
		addErrorPage(getIndexArg(idx, 1), getIndexArg(idx, 2));
		size_t previous_idx = idx;
		while (doesLineExist("error_page", idx, previous_idx)) {
			addErrorPage(getIndexArg(idx, 1), getIndexArg(idx, 2));
			previous_idx = idx;
		}
	}
	if (doesLineExist("max_client_body_size", idx))
		_max_client_body_size = std::stoi(getIndexArg(idx, 1));
}

bool Server::addErrorPage(const std::string nbr, const std::string file_path) {
	_error_pages.insert({ std::stoi(nbr), file_path });
	return true;
}

//deprecated
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
	for (const Location &loc : _locations)
		if (loc._path == location._path)
			throw std::runtime_error("Server cannot have two matching locations!");
	_locations.push_back(location);
}

bool Server::matchRequest(const std::string server_name, const size_t port) {
	return (_listen_name.find(server_name) == 0 && port == _port);
}

bool Server::matchRequest(const Request &request) {
	//std::cout << "server matchRequest called for server:" << _listen_name << std::endl;
	if (_listen_name == "*") return true;
	if (request._port != _port)
		return false;
	if (_listen_name == request._host)
		return true;
	for (const std::string &s : _server_names) {
		if (s == request._host)
			return true;
	}
	return false;
}

Response Server::resolveRequest(const Request &request) {
	std::string filepath;
	//std::cout << "resolverRequest called, ws_getlastchar: " << ws_getlastchar('/', request._path) << std::endl;
	if (ws_getlastchar('/', request._path) != 0) {
		size_t last_slash = ws_getlastchar('/', request._path);
		for (Location &loc : _locations) {
			//std::cout << "rR matching location: " << loc._path << " last_slash:" << last_slash << " locpathsize:" << loc._path.size() << std::endl;
			if (loc._path.size() == last_slash + 1 && loc._path.at(last_slash) == '/' && loc.requestMatch(request, filepath))
				return (Response(filepath));
	}	}
	for (Location &loc : _locations) {
		//std::cout << "rR searching non-/ locations" << std::endl;
		if (loc._path != "/" && loc.requestMatch(request, filepath))
			return (Response(filepath));
	}
	for (Location &loc : _locations) {
		//std::cout << "rR searching / location" << std::endl;
		if (loc._path == "/" && loc.requestMatch(request, filepath))
			return (Response(filepath));
	}
	return Response(404, getErrorPage(404));
}

// searches _error_pages for given error number, returns string if found or empty string if not
std::string Server::getErrorPage(const size_t page_num) {
	try {
		return _error_pages.at(page_num);
	} catch (const std::exception &e) {
		return "";
	}
}

void Server::printData() {
	std::cout << "\e[0;32mServer.printData() : \e[0;92m" << _listen_name << ":" << _port << "\e[0;32m" << std::endl;
	if (_server_names.size() > 0) std::cout << "Names: ";
	for (const std::string &s : _server_names)
		std::cout << s << " | ";
	if (_server_names.size() > 0) std::cout << std::endl;
	if (_error_pages.size() > 0) std::cout << "Error pages:" << std::endl;
	if (_max_client_body_size > 0) std::cout << "Max client body size: " << _max_client_body_size << std::endl;
	for (const std::pair<const size_t, std::string> &p : _error_pages)
		std::cout << p.first << ":" << p.second << std::endl;
	for (Location &loc : _locations)
		loc.printData();
	std::cout << "\e[0m";
}