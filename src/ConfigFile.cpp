#include "ConfigFile.hpp"

ConfigFile::ConfigFile(const std::string &config_file) : _file(config_file) {
	if (!_file)
		throw std::runtime_error("Error opening configuration file: " + config_file);
	if (!nextLine())
		throw std::runtime_error("Empty configuration file");
	_sections.push_back(GLOBAL);
}

ConfigFile::~ConfigFile() {
	_file.close();
}

bool ConfigFile::emptyLine() {
	size_t pos = 0;
	while (!ws_endl(_line, pos) && ws_wspace(_line.at(pos)))
		pos++;
	return (ws_endl(_line, pos));
}

bool ConfigFile::nextLine() {
	if (!std::getline(_file, _line))
		return false;
	while (emptyLine())
		if (!std::getline(_file, _line))
			return false;
	//std::cout << "CF::nextLine [" << getSection() << "]" << getWord(0) << ":" << getWord(1) << std::endl;
	return true;
}

bool ConfigFile::processLine() {
	_words.clear();
	size_t pos = 0;
	//size_t idx = std::string::npos;
	std::string word;
	while (!ws_endl(_line, pos) && ws_wspace(_line.at(pos)))
		pos++;
	if (ws_endl(_line, pos) || !(_line.at(pos) == '}' || ws_keyword_char(_line.at(pos)))) {
		//std::cout << _line << "[pos= " << pos << "|ws_kw_c:" << ws_keyword_char(_line.at(pos)) << "]" << std::endl;
		throw std::runtime_error("Invalid character found at beginning of configuration line");
	}
	if (_line.at(pos) == '}' && ++pos) {
		while (!ws_endl(_line, pos) && ws_wspace(_line.at(pos)))
			pos++;
		if (!ws_endl(_line, pos))
			throw std::runtime_error("Invalid characters present on line after section end");
		if (_sections.size() <= 1)
			throw std::runtime_error("Section ending when no section has begun");
		return (_words.push_back("}"),_sections.pop_back(), true);
	}
	while (!ws_endl(_line, pos) && ws_keyword_char(_line.at(pos)))
		word.push_back(_line.at(pos++));
	if (!ws_endl(_line, pos) && !ws_wspace(_line.at(pos)))
		throw std::runtime_error("Invalid characters present after keyword in configuration file");
	ws_get_next_nonws(_line, pos);
	_words.push_back(word);
	while (!ws_endl(_line, pos)) {
		if (!processParam(pos) && !ws_endl(_line, pos))
			throw std::runtime_error("Error processing arguments at line: " + _words.at(0));
		pos++;
	}
	//std::cout << "words.back = " + _words.back() << std::endl;
	if (_words.back() == "{")
		_sections.push_back(_words.at(0));
	return true;
	//ws_get_next_nonws(_line, pos);
}

const std::string &ConfigFile::getSection() { 
	if (_sections.empty())
		return empty;
	return _sections.back();
}

const std::string &ConfigFile::getWord(const size_t &num) {
	return ((num < _words.size() ? _words.at(num) : empty));
	//if (num < _words.size())
	//	return _words.at(num);
	//return empty;
}

const std::string &ConfigFile::getLastWord() { return _words.back(); }

std::vector<std::string> ConfigFile::getVector() { 
	std::vector<std::string> vector = _words;
	vector.insert(vector.begin(), getSection());
	return vector; 
}

bool ConfigFile::processParam(size_t &pos) {
	bool quotes = false;
	std::string param;
	while ((!quotes && !ws_endl(_line, pos) && !ws_wspace(_line.at(pos)) && std::isprint(_line.at(pos)))
			|| (quotes && pos < _line.size() && std::isprint(_line.at(pos)))) {
				if (_line.at(pos) == '"')
					quotes = (!quotes) ? true : false;
				param.push_back(_line.at(pos++));
			}
	if (quotes)
		throw std::runtime_error("Unmatched quotes at keyword: " + _words.at(0));
	//std::cout << "push_back param: " << param << std::endl;
	_words.push_back(param);
	return true;
}

void ConfigFile::print() {
	for (const std::string &s : _sections)
		std::cout << "/" << s;
	std::cout << ":";
	for (const std::string &s : _words)
		std::cout << "[" << s << "]";
	std::cout << std::endl;
}