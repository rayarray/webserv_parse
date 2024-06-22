#include "ws_functions.hpp"
#include "Server.hpp"

Server::Server() : ConfigSection("server"), _max_client_body_size(0) {}

// interprets values from the base ConfigSection class and stores them in Server variables
void Server::initialize() {
	size_t idx, previous_idx, first = 1;
	while ((first && doesLineExist("listen", idx)) || (!first && doesLineExist("listen", idx, previous_idx))) {
		for (size_t i = 1; !getIndexArg(idx, i).empty(); i++) {
			size_t const new_port = std::stoi(getIndexArg(idx, i));
			if (matchPort(new_port))
				throw ServerException("Duplicate ports in server");
			if (new_port == 0 || new_port > 65535)
				throw ServerException("Invalid port specified");
			_ports.push_back(new_port);
		}
		previous_idx = idx;
		first = 0;
	}
	if (_ports.empty()) _ports.push_back(80);
	if (doesLineExist("server_name", idx)) {
		for (size_t i = 1; !getIndexArg(idx, i).empty(); i++)
			_server_names.push_back(getIndexArg(idx, i));
	}
	for (const std::string &s : _server_names)
		if (s == "*" && _server_names.size() != 1)
			throw ServerException("Server name wildcard '*' used, but other server names also present");
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

void Server::addLocation(Location location) {
	for (const Location &loc : _locations)
		if (loc._path == location._path)
			throw ServerException("Server cannot have two matching locations!");
	_locations.push_back(location);
}

bool Server::matchRequest(const Request &request) {
	if (!matchPort(request._port))
		return false;
	if (_server_names.size() == 1 && _server_names.at(0) == "*") return true;
	for (const std::string &s : _server_names) {
		if (s == request._host)
			return true;
	}
	return false;
}

Response Server::resolveRequest(const Request &request) {
	std::string filepath;
	if (ws_getlastchar('/', request._path) != 0) {
		size_t last_slash = ws_getlastchar('/', request._path);
		for (Location &loc : _locations) {
			if (loc._path.size() == last_slash + 1 && loc._path.at(last_slash) == '/' && loc.requestMatch(request, filepath))
				return (Response(filepath));
	}	}
	for (Location &loc : _locations) {
		if (loc._path != "/" && loc.requestMatch(request, filepath))
			return (Response(filepath));
	}
	for (Location &loc : _locations) {
		if (loc._path == "/" && loc.requestMatch(request, filepath))
			return (Response(filepath));
	}
	return Response(404, getErrorPage(404));
}

bool Server::resolveLocation(int const method, std::string const &request_path, size_t &index) {
	size_t match_size = 0, new_match_size = 0;
	for (size_t i = 0; i < _locations.size(); i++) {
		if (_locations.at(i).requestMatch(method, request_path, new_match_size) && new_match_size > match_size) {
			match_size = new_match_size;
			index = i;
	}	}
	return (match_size);
}

Response Server::resolveRequest(int const method, std::string const &request_path) {
	size_t match_index;
	if (!resolveLocation(method, request_path, match_index))
		return (Response(404, getErrorPage(404)));
	std::string root_path = _locations.at(match_index).makeRootPath(request_path);
	if (request_path.back() == '/' && root_path.back() == '/')
		return (Response(RES_DIR, root_path));
	
	if (std::string cgi_path; _locations.at(match_index).checkCGI(request_path, cgi_path))
		return (Response(RES_CGI, root_path, cgi_path));
	return (Response(RES_FILE, root_path));
}

// searches _error_pages for given error number, returns string if found or empty string if not
std::string Server::getErrorPage(const size_t page_num) {
	try {
		return _error_pages.at(page_num);
	} catch (const std::exception &e) {
		return "";
	}
}

bool Server::matchPort(const size_t port) {
	for (size_t const &_port : _ports)
		if (port == _port) return true;
	return false;
}

void Server::printData() {
	std::cout << "\e[0;32mServer.printData() : Ports \e[0;92m";
	for (size_t const &port : _ports)
		std::cout << "[" << port << "]";
	std::cout << "\e[0;32m" << std::endl;
	if (_server_names.size() > 0) std::cout << "Server name: \e[0;92m";
	for (const std::string &s : _server_names)
		std::cout << "[" << s << "]";
	if (_server_names.size() > 0) std::cout << "\e[0;32m" << std::endl;
	if (_error_pages.size() > 0) std::cout << "Error pages:" << std::endl;
	if (_max_client_body_size > 0) std::cout << "Max client body size: " << _max_client_body_size << std::endl;
	for (const std::pair<const size_t, std::string> &p : _error_pages)
		std::cout << p.first << ":" << p.second << std::endl;
	for (Location &loc : _locations)
		loc.printData();
	std::cout << "\e[0m";
}

std::string const Server::printId() const {
	std::string id("");
	if (!_server_names.empty())
		id.append(_server_names.at(0));
	for (size_t const &p : _ports)
		id.append(":" + std::to_string(p));
	return id;
}