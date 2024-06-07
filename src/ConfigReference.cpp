#include "ConfigReference.hpp"

ConfigReference::ConfigReference(const std::string &reference_file) : _section(GLOBAL) {
	std::ifstream file(reference_file);
	std::string line;
	if (!file)
		throw std::runtime_error("Error opening configuration reference file!");
	if (!std::getline(file, line) || line != "# INTERNAL FILE, DO NOT MODIFY")
		throw std::runtime_error("Invalid configuration reference file!");
	while (std::getline(file, line))
		if (!processLine(line))
			throw std::runtime_error("Invalid line in configuration reference file!");
	file.close();
	if (!keyExists(GLOBAL, "server") || !keyExists("server", "listen")) 
		throw std::runtime_error("Configuration reference file is missing server keyword or section!");
}

ConfigReference::~ConfigReference() {}

bool ConfigReference::keyExists(const std::string section, const std::string keyword) {
	for (std::vector<std::string> ref_line : _references) {
		if (ref_line.size() >= 2 && ref_line.at(0) == section && ref_line.at(1) == keyword)
			return true;
	}
	return false;
}

bool ConfigReference::keyExists(const std::string section, const std::string keyword, size_t &index) {
	for (size_t i = 0; i < _references.size(); i++) {
		if (_references.at(i).size() >= 2 && _references.at(i).at(0) == section && _references.at(i).at(1) == keyword)
			return (index = i, true);
	}
	return false;
}

// gets parameter number x, first is 0
char ConfigReference::keyParamType(const std::string section, const std::string keyword, const size_t param_num) {
	size_t pnum = param_num + 2;
	size_t idx;
	if (!keyExists(section, keyword, idx))
		throw std::runtime_error("ConfigReference::keyParamType called with unmatched keyword!");
	if (pnum >= _references.at(idx).size()) {
		if (_references.at(idx).size() > 2 && _references.at(idx).back().size() == 2)
			return _references.at(idx).back().at(0);
		else
			return 0;
	}
	return (_references.at(idx).at(pnum).at(0));
}

bool ConfigReference::keyParamTypeMatch(const std::string section, const std::string keyword, const size_t param_num, const char type) {
	const char ptype = keyParamType(section, keyword, param_num);
	if (!validType(ptype) || !validType(type))
		throw std::runtime_error("Invalid configuration parameter type encountered!");
	if (type == ptype)
		return true;
	return false;
}

bool ConfigReference::processLine(const std::string &line) {
	size_t pos = 0;
	std::vector<std::string> processed_line;
	std::string processed_word;
	if (!ws_endl(line, pos) && std::isupper(line.at(pos))) {
		for (;!ws_endl(line, pos) && !ws_wspace(line.at(pos)) && ws_keyword_char(line.at(pos)); pos++)
			processed_word.push_back(line.at(pos));
		_section = ws_tolower(processed_word);
		return true;
	}
	processed_line.push_back(_section);
	for (;!ws_endl(line, pos) && !ws_wspace(line.at(pos)) && ws_keyword_char(line.at(pos)); pos++)
		processed_word.push_back(line.at(pos));
	if (processed_word.size() < 2)
		throw std::runtime_error("Invalid reference keyword: " + processed_word);
	processed_line.push_back(processed_word);
	if (ws_endl(line, pos))
		return true;
	while (!ws_endl(line, pos) && ws_wspace(line.at(pos)) && !ws_endl(line, ++pos) && validType(line.at(pos))) {
		processed_word.assign(1, line.at(pos));
		processed_line.push_back(processed_word);
		if (!ws_endl(line, pos + 1) && line.at(pos + 1) == processed_word.at(0) && ws_endl(line, pos + 2) && ++pos && ++pos) {
			processed_line.back().push_back(processed_word.at(0));
			break;
		}
		pos++;
	}
	if (!ws_endl(line, pos))
		throw std::runtime_error("Invalid syntax in configuration reference file!");
	return (_references.push_back(processed_line), true);
}

inline bool ConfigReference::validType(const char type) {
	return (type == 'S' || type == 'N' || type == 'T');
}

void ConfigReference::print() {
	std::cout << "printing configref:" << std::endl;
	for (std::vector<std::string> &ref : _references) {
		std::cout << "[";
		for (std::string &item : ref)
			std::cout << item << ":";
		std::cout << "]" <<std::endl;
	}
}