#include <iostream> // for debug
#include "ConfigParser.hpp"

// usage: create instance with config file path, 
// start parsing with startParse which returns true if valid conf
// get servers with getServer, returns false when last server reached

inline bool ws_wspace(char c) { return (c == ' ' || c == '\t'); }

ConfigParser::ConfigParser(const std::string &filepath) : _cfile(filepath), _pos(0) {}

ConfigParser::~ConfigParser() {
	if (_cfile)
		_cfile.close();
}

bool ConfigParser::startParse() { (void)_pos;
	if (!_cfile) return (_error = "Error opening config file", false);
	std::getline(_cfile, _line);
	while (skipWhiteSpaceLines()) {
		if (!handleConfig())
			return (checkServer());
	}
	_error = _line;
	return true;
}


bool ConfigParser::getServer(Server &server) { (void)server;
	return true;
}

bool ConfigParser::endParse() {
	return true;
}

bool ConfigParser::handleConfig() {
	std::cout << "handleC _pos[" << _pos << "] size[" << _line.size() << "]" << std::endl;
	if (_pos >= _line.size()) return false;
	size_t _end = _pos;
	while (_end < _line.size() && !ws_wspace(_line.at(_end))) {
		_end++;
	}
	return (std::cout << "handleConf: [" << _line.substr(_pos, _end - _pos) << std::endl, _pos = _end, true);
}

bool ConfigParser::checkServer() {
	return true;
}

// returns true if no parseable content left, else false
bool ConfigParser::skipWhiteSpace() {
	while (_pos < _line.size() && ws_wspace(_line.at(_pos))) { _pos++; }
	if (_pos >= _line.size() || _line.at(_pos) == '#')
		return (_pos = _line.size(), true);
	return false;
}

// returns true if reached end of config file
bool ConfigParser::skipWhiteSpaceLines() {
	while (skipWhiteSpace()) {
		_pos = 0;
		if (!std::getline(_cfile, _line)) {
			//std::cout << "skipWSL: false" << std::endl;
			return true;
		}
	}
	//std::cout << "skipWSL: false" << std::endl;
	return false;
}