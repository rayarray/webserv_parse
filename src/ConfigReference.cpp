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
	//if (!keyExists("server") || !keyExists("server", "server")) 
	//	throw std::runtime_error("Configuration reference file is missing server keyword or section!");
}

ConfigReference::~ConfigReference() {}

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
		//std::cout << "to_string: " << processed_word << std::endl;
		processed_line.push_back(processed_word);
		//std::cout << "pushed: " << line.at(pos) << ", rest of line: " << line.substr(pos, -1) << std::endl;
		//std::cout << "ws_endl(line, pos + 1) = " << std::boolalpha << ws_endl(line, pos + 1) << std::endl;
		//std::cout << "processed_word.at(0) = " << processed_word.at(0) << std::endl;
		//std::cout << "ws_endl(line, pos + 2) = " << std::boolalpha << ws_endl(line, pos + 2) << std::endl;
		if (!ws_endl(line, pos + 1) && line.at(pos + 1) == processed_word.at(0) && ws_endl(line, pos + 2) && ++pos && ++pos) {
			processed_line.back().push_back(processed_word.at(0));
			break;
		}
		pos++;
	}
	if (!ws_endl(line, pos))
		throw std::runtime_error("Invalid syntax in configuration reference file!");
	// std::cout << "processed_line: [";
	// for (std::string &i : processed_line)
	// 	std::cout << i << ":";
	// std::cout << std::endl;
	return (_references.push_back(processed_line), true);
}

inline bool ConfigReference::validType(const char type) {
	return (type == 'S' || type == 'C' || type == 'N' || type == 'T');
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