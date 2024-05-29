#include <iostream> // for debug
#include "ws_functions.hpp"
#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &filepath) : _cfile(filepath), _pos(0) {}

ConfigParser::~ConfigParser() {
	if (_cfile) _cfile.close();
}

bool ConfigParser::getConfigSets() {
	std::ifstream config_file(CONFIG_DEFAULTS);
	if (!config_file) return (_error = "Error opening config values file", false);
	if (!std::getline(config_file, _line) || _line != "# INTERNAL FILE, DO NOT MODIFY") 
		return (_error = "Invalid config defaults file", false);
	while (std::getline(config_file, _line))
		config_defaults.push_back(_line);
	for (const std::string& line : config_defaults) { // print config_defaults
		std::cout << line << std::endl;
	}
	return true;
}

bool ConfigParser::startParse() {
	if (!_cfile) return (_error = "Error opening config file", false);
	if (!getConfigSets()) return (false);
	std::getline(_cfile, _line);
	while (!skipWhiteSpaceLines()) {
		if (!handleConfig())
			return (_error = "Config error: " + _error, checkServer());
	}
	_error = _line;
	return true;
}


bool ConfigParser::getServer(Server &server) { (void)server;
	return true;
}

bool ConfigParser::endParse() {
	if (_cfile) _cfile.close();
	return true;
}

// bool ConfigParser::handleConfig() {
// 	std::cout << "handleC _pos[" << _pos << "] size[" << _line.size() << "]" << std::endl;
// 	if (_pos >= _line.size()) return false;
// 	size_t _end = _pos;
// 	while (_end < _line.size() && !ws_wspace(_line.at(_end))) {
// 		_end++;
// 	}
// 	return (std::cout << "handleConf: [" << _line.substr(_pos, _end - _pos) << "]" << std::endl, _pos = _line.size(), true);
// }

bool ConfigParser::handleConfig() {
	if (ws_checkword(_line.substr(_pos, std::string::npos), config_defaults))
		return (std::cout << "handleconf: true" << std::endl, _pos = _line.size(), true);
	return (std::cout << "handleconf: false" << std::endl, _pos = _line.size(), false);
}

bool ConfigParser::checkServer() {
	return false;
}

// skips white space characters and jumps to end on comment character(#) on stored _line
// returns true if no parseable content left, else false
bool ConfigParser::skipWhiteSpace() {
	while (_pos < _line.size() && ws_wspace(_line.at(_pos))) { _pos++; }
	if (_pos >= _line.size() || _line.at(_pos) == '#')
		return (_pos = _line.size(), true);
	return false;
}

// skips whitespaces and comments on line or multiple lines
// returns true if reached end of config file
bool ConfigParser::skipWhiteSpaceLines() {
	while (skipWhiteSpace()) {
		_pos = 0;
		if (!std::getline(_cfile, _line)) {
			//std::cout << "skipWSL: true" << std::endl;
			return true;
		}
	}
	//std::cout << "skipWSL: false" << std::endl;
	return false;
}