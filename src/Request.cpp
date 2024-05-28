#include "Request.hpp"

Request::Request() : _responsecode(REQ_INVALID) {} // invalid, only used for placeholder

Request::Request(const std::string filepath) : _responsecode(REQ_FILE), _path(filepath) {}

Request::~Request() {}

const std::string &Request::getPath() {
	return _path;
}

Request &Request::operator=(const Request assign) {
	if (this == &assign)
		return *this;
	this->~Request();
	new (this) Request(assign);
	return *this;
}