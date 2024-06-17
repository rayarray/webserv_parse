#include "Location.hpp"
#include "ws_functions.hpp"

Location::Location(const std::string path) : ConfigSection("location"), _path(path), _get(false), _post(false), _del(false), _dir_list(false) {}

void Location::initialize() {
	size_t idx = 0;
	//if (doesLineExist("methods")) std::cout << "METHODS ARE PRESENT" << std::endl;
	if (doesLineExist("methods", idx)) {
		for (size_t i = 1; !getIndexArg(idx, i).empty(); i++) {
			if (getIndexArg(idx, i) == "GET")
				_get = true;
			else if (getIndexArg(idx, i) == "POST")
				_post = true;
			else if (getIndexArg(idx, i) == "DELETE")
				_del = true;
			else
				throw std::runtime_error("Invalid method specified in location!");
		}
	} else {
		_get = true;
	}
	if (doesLineExist("root", idx))
		_rootpath = getIndexArg(idx, 1);
	if (doesLineExist("rewrite", idx))
		_rewrite = getIndexArg(idx, 1);
	if (doesLineExist("directory_index", idx) && getIndexArg(idx, 1) == "yes")
		_dir_list = true;
	if (doesLineExist("default_index", idx))
		_index_file = getIndexArg(idx, 1);
}

bool Location::requestMatch(const std::string request, std::string &filepath) {
	std::cout << "DEPRECATED location.requestmatch called, path[" << _path << "] req[" << request << "]" << std::endl;
	if (request.find(_path) == 0)
		return (filepath = _rootpath + request, true);
	return false;
}

bool Location::requestMatch(const Request &request, std::string &filepath) {
	//std::cout << "BREAK" << std::endl;
	if (request._path.find(_path) == 0 && methodAvailable(request._method))
		return (filepath = _rootpath + request._path.substr(_path.size() - 1, std::string::npos), true);
	return false;
}

void Location::printData() {
	std::cout << "\e[0;33mLocation.printData() : \e[0;93m" << _path << std::endl;
	std::cout << "\e[0;33mMethods: ";
	if (_get) std::cout << "GET ";
	if (_post) std::cout << "POST ";
	if (_del) std::cout << "DELETE";
	std::cout << std::endl;
	if (!_rewrite.empty()) std::cout << "Rewrite: " << _rewrite << std::endl;
	if (!_rootpath.empty()) std::cout << "Root: " << _rootpath << std::endl;
	std::cout << "Directory listing: " << std::boolalpha << _dir_list << std::endl;
	if (!_index_file.empty()) std::cout << "Dir index file: " << _index_file << std::endl;
	std::cout << "\e[0m";
}

bool Location::methodAvailable(const int method) {
	//std::cout << "methodAvailable called, get:" << std::boolalpha << _get << " post:" << _post << " del:" << _del << std::endl;
	if (_get && method == REQ_GET) return true;
	if (_post && method == REQ_POST) return true;
	if (_del && method == REQ_DEL) return true;
	return false;
}