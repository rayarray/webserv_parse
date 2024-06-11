#include "Location.hpp"

Location::Location(const std::string path) : ConfigSection("location"), _path(path), _get(false), _post(false), _del(false), _dir_list(false) {}

void Location::initialize() {
	size_t idx = 0;
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