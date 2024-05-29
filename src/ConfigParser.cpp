#include <iostream> // for debug
#include "ws_functions.hpp"
#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &filepath) : _cfile(filepath), _pos(0), _server_index(0) {}

ConfigParser::~ConfigParser() {
	if (_cfile) _cfile.close();
}

bool ConfigParser::getConfigSets() {
	std::ifstream config_file(CONFIG_DEFAULTS);
	if (!config_file) return (_error = "Error opening config values file", false);
	if (!std::getline(config_file, _line) || _line != "# INTERNAL FILE, DO NOT MODIFY") 
		return (_error = "Invalid config defaults file", false);
	while (std::getline(config_file, _line))
		_config_defaults.push_back(_line);
	//for (const std::string& line : _config_defaults) { // print _config_defaults
	//	std::cout << line << std::endl;
	//}
	if (!ws_checkword("SERVER", _config_defaults, _server_index))
		return (_error = "Invalid config defaults file", false);
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
	size_t cfg_index;
	if (ws_checkword_lower(_line.substr(_pos, std::string::npos), _config_defaults, cfg_index)) {
		std::cout << "handleConfig match, index " << cfg_index << ", word " << _config_defaults.at(cfg_index) << std::endl;
		if (_config_defaults.at(cfg_index).back() == 'S' && !handleSubConfig(cfg_index))
			return false;
		else if (handleConfig(cfg_index))
			return false;
		_pos = _line.size();
		return true;
	}
	return false;
}

bool ConfigParser::handleConfig(size_t index) { (void)index;
	return true;
}

bool ConfigParser::handleSubConfig(size_t index) { (void)index;
	std::cout << "handleSubConfig called" << std::endl;
	return true;
}

bool ConfigParser::checkSyntax(size_t index) {
	size_t wordsize = _config_defaults.at(index).find_first_of(" ");
	if (wordsize == 0 || wordsize == std::string::npos)
		return (_error = "Unexpected syntax in config defaults!", false);
	size_t def_pos = wordsize;
	size_t cfg_pos = wordsize + _pos;
	while (def_pos + 1 < _config_defaults.at(index).size() && cfg_pos + 1 < _line.size()) {
		if (!ws_wspace(_line.at(cfg_pos)))
			return (_error = "Whitespace not found in expected position at keyword [" + _config_defaults.at(index).substr(0, wordsize) + "]", false);
		else if (!ws_wspace(_config_defaults.at(index).at(def_pos)))
			return (_error = "Expected whitespace not found in config defaults!", false);
		def_pos++;
		cfg_pos++;
		
	}
}

bool ConfigParser::checkServer() {
	if (_line.substr(_pos, std::string::npos).find("server") == 0)
		return true;
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
		if (!std::getline(_cfile, _line))
			return true;
	}
	return false;
}