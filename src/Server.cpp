#include "ws_functions.hpp"
#include "Server.hpp"

Server::Server(const std::string listen, const size_t port) : _listen_name(listen), _port(port) {}

bool Server::matchRequest(const std::string server_name, const size_t port) {
	return (_listen_name.find(server_name) == 0 && port == _port);
}

Request Server::resolveRequest(const std::string request) {
	return Request("/usr/share/www" + request);
}