#include "Response.hpp"

Response::Response() : _responsecode(REQ_INVALID) {} // invalid, only used for placeholder

Response::Response(const std::string filepath) : _responsecode(REQ_FILE), _path(filepath) {}

Response::Response(const int code, const std::string filepath) : _responsecode(code), _path(filepath) {}

Response::~Response() {}

const int &Response::getType() { return _responsecode; }

const std::string &Response::getPath() { return _path; }

Response &Response::operator=(const Response assign) {
	if (this == &assign)
		return *this;
	_path = assign._path;
	_responsecode = assign._responsecode;
	return *this;
}